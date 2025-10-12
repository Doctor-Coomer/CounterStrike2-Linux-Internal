#include <unistd.h>
#include <dlfcn.h>
#include <sys/mman.h>

#include "math.hpp"

#include "interfaces/input_system.hpp"
#include "interfaces/game_entity_system.hpp"
#include "interfaces/engine.hpp"
#include "interfaces/cvar_system.hpp"

#include "funchook/funchook.h"
#include "libsigscan/libsigscan.h"

#include "hooks/sdl.cpp"
#include "hooks/vulkan.cpp"

#include "hooks/input_create_move.cpp"
#include "hooks/allow_camera_angle_change.cpp"

#include "hooks/something.cpp"

#include "print.hpp"


void* get_interface(const char* lib_path, const char* version) {
  void* lib_handle = dlopen(lib_path, RTLD_NOLOAD | RTLD_NOW);
  if (!lib_handle) {
    print("Failed to load %s\n", lib_path);
    return NULL;
  }

  print("%s loaded at %p\n", lib_path, lib_handle);
  
  typedef void* (*CreateInterface)(const char*, int*);

  CreateInterface create_interface = (CreateInterface)dlsym(lib_handle, "CreateInterface");
  dlclose(lib_handle);
  
  if (!create_interface) {
    print("Failed to get CreateInterface\n");
    return NULL;
  }

  print("%s factory found at %p\n", lib_path, create_interface);

  void* interface =  create_interface(version, NULL);
    
  if (!interface) {
    print("Failed to get %s interface\n", version);
    return NULL;
  }

  print("%s interface found at %p\n", version, interface);
    
  return interface;
}


bool write_to_table(void** vtable, int index, void* func) {

  const long page_size = sysconf(_SC_PAGESIZE);

  // Sets last three digits to zero
  void* table_page = (void*)((__uint64_t)vtable & ~(page_size - 1));
  print("vfunc table page found at %p\n", table_page);

  if (mprotect(table_page, page_size, PROT_READ | PROT_WRITE) != 0) {
    print("mprotect failed to change page protection\n");
    return false;
  }

  vtable[index] = func;

  if (mprotect(table_page, page_size, PROT_READ) != 0) {
    print("mprotect failed to reset page protection\n");
    return false;
  }

  return true;
}


void** input_vtable;

funchook_t* funchook;

//call ((void* (*)(const char*, int))dlopen)("/home/DrCoomer/Programming/c++/CounterStrike2-Linux-Internal/cs2.so",1)
//call ((const char* (*)(void))dlerror)()

__attribute__((constructor))
void entry() {  

  // Sig go crazy
  unsigned long func_addr1 = (unsigned long)sigscan_module("libclient.so", "48 8D 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC 55 48 89 E5 41 55 41 54 49 89 FC 53 48 89 F3 BE ? ? ? ? 48 81 EC ? ? ? ? E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 4C 8D AD ? ? ? ? 4C 89 E6 BA ? ? ? ? 4C 8D 25 ? ? ? ? 4C 89 EF E8 ? ? ? ? 4C 89 E6 4C 89 EF E8 ? ? ? ? 48 89 C6 48 85 C0 75 ? EB ? 0F 1F 40");
  unsigned int input_eaddr = *(unsigned int*)(func_addr1 + 0x3);
  unsigned long next_instruction1 = (unsigned long)(func_addr1 + 0x7);
  input = (Input*)((void*)(next_instruction1 + input_eaddr));

  print("CInput: %p\n", input);
  
  // https://github.com/avitran0/deadlocked/blob/406ba57037ad4462740183396dad4fa5e12468c5/src/cs2/mod.rs#L360
  unsigned long func_addr2 = (unsigned long)sigscan_module("libclient.so", "4C 8D 05 ? ? ? ? 48 8D 0D ? ? ? ? 48 8B 38");
  unsigned int view_matrix_eaddr = *(unsigned int*)(func_addr2 + 0x3);
  unsigned long next_instruction2 = (unsigned long)(func_addr2 + 0x7);
  view_matrix = ((float(*)[4][4])(next_instruction2 + view_matrix_eaddr));

  // https://github.com/avitran0/deadlocked/blob/rust/src/cs2/mod.rs#L346
  unsigned long func_addr3 = (unsigned long)sigscan_module("libclient.so", "48 83 3D ? ? ? ? 00 0F 95 C0 C3");
  unsigned int localentity_eaddr = *(unsigned int*)(func_addr3 + 0x3);
  unsigned long next_instruction3 = (unsigned long)(func_addr3 + 0x8);
  localentity_ptr = ((Entity**)(next_instruction3 + localentity_eaddr));
  
  engine = (Engine*)get_interface("libengine2.so", "Source2EngineToClient001");
  cvar_system = (CvarSystem*)get_interface("libtier0.so", "VEngineCvar007");
  
  void* game_resource_service = get_interface("libengine2.so", "GameResourceServiceClientV001");
  entity_system = (GameEntitySystem*)*(void**)((unsigned long)game_resource_service + 0x50);
  
  input_vtable = *(void***)input;
  
  input_create_move_original = (bool (*)(void*, int, bool))input_vtable[6];  
  if (!write_to_table(input_vtable, 6, (void*)input_create_move_hook)) {
    print("Input::CreateMove hook failed\n");
  } else {
    print("Input::CreateMove hooked\n");
  }

  allow_camera_angle_change_original = (void (*)(void*, int))input_vtable[8];
  if (!write_to_table(input_vtable, 8, (void*)allow_camera_angle_change_hook)) {
    print("Input::AllowCameraAngleChange hook failed\n");
  } else {
    print("Input::AllowCameraAngleChange hooked\n");
  }
  
  funchook = funchook_create();

  int rv;
  
  something_original = (unsigned long (*)(unsigned long, unsigned long))sigscan_module("libclient.so", "41 56 41 55 41 54 55 48 89 FD 53 48 89 F3 BE");
  rv = funchook_prepare(funchook, (void**)&something_original, (void*)something_hook);
  if (rv != 0) {
  }
  
  void* lib_vulkan_handle = dlopen("/run/host/usr/lib/libvulkan.so.1", RTLD_LAZY | RTLD_NOLOAD);
  if (lib_vulkan_handle == nullptr)
    lib_vulkan_handle = dlopen("/run/host/usr/lib64/libvulkan.so.1", RTLD_LAZY | RTLD_NOLOAD); // Some distributions have a lib64 directory instead

  if (lib_vulkan_handle == nullptr) print("Can't find libvulkan!\n");
  
  if (lib_vulkan_handle != nullptr) {
    print("Vulkan loaded at %p\n", lib_vulkan_handle);

    // https://github.com/bruhmoment21/UniversalHookX/blob/main/UniversalHookX/src/hooks/backend/vulkan/hook_vulkan.cpp#L47
    VkInstanceCreateInfo create_info = {};
    constexpr const char* instance_extension = "VK_KHR_surface";
      
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.enabledExtensionCount = 1;
    create_info.ppEnabledExtensionNames = &instance_extension;
  
    // Create Vulkan Instance without any debug feature
    vkCreateInstance(&create_info, vk_allocator, &vk_instance);
  
    uint32_t gpu_count;
    vkEnumeratePhysicalDevices(vk_instance, &gpu_count, NULL);
    IM_ASSERT(gpu_count > 0);

    VkPhysicalDevice* gpus = new VkPhysicalDevice[sizeof(VkPhysicalDevice) * gpu_count];
    vkEnumeratePhysicalDevices(vk_instance, &gpu_count, gpus);

    // If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
    // most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
    // dedicated GPUs) is out of scope of this sample.
    int use_gpu = 0;
    for (int i = 0; i < (int)gpu_count; ++i) {
      VkPhysicalDeviceProperties properties;
      vkGetPhysicalDeviceProperties(gpus[i], &properties);
      if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
	use_gpu = i;
	break;
      }
    }
	
    vk_physical_device = gpus[use_gpu];

    delete[] gpus;

    vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device, &count, NULL);

    queue_families = (VkQueueFamilyProperties*)malloc(count*sizeof(VkQueueFamilyProperties));

    vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device, &count, queue_families);

    for (uint32_t i = 0; i < count; ++i) {
      if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
	queue_family = i;
	break;
      }
    }
  
    if (queue_family == (uint32_t)-1) {
      print("queue_family fail\n");
    }
  
    constexpr const char* device_extension = "VK_KHR_swapchain";
    constexpr const float queue_priority = 1.0f;

    VkDeviceQueueCreateInfo queue_info = { };
    queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_info.queueFamilyIndex = queue_family;
    queue_info.queueCount = 1;
    queue_info.pQueuePriorities = &queue_priority;

    VkDeviceCreateInfo create_info2 = { };
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info2.queueCreateInfoCount = 1;
    create_info2.pQueueCreateInfos = &queue_info;
    create_info.enabledExtensionCount = 1;
    create_info.ppEnabledExtensionNames = &device_extension;

    VkDevice vk_fake_device = VK_NULL_HANDLE;

    vkCreateDevice(vk_physical_device, (const VkDeviceCreateInfo*)&create_info, vk_allocator, &vk_fake_device);
    if (vk_fake_device == nullptr) {
      print("Failed to create Vulkan dummy device\n");
      return;
    }
      
    queue_present_original = (VkResult (*)(VkQueue, const VkPresentInfoKHR*))vkGetDeviceProcAddr(vk_fake_device, "vkQueuePresentKHR");
    acquire_next_image_original = (VkResult (*)(VkDevice, VkSwapchainKHR, uint64_t, VkSemaphore, VkFence, uint32_t*))vkGetDeviceProcAddr(vk_fake_device, "vkAcquireNextImageKHR");
    acquire_next_image2_original = (VkResult (*)(VkDevice, const VkAcquireNextImageInfoKHR*,  uint32_t*))vkGetDeviceProcAddr(vk_fake_device, "vkAcquireNextImage2KHR");
    create_swapchain_original = (VkResult (*)(VkDevice, const VkSwapchainCreateInfoKHR*, const VkAllocationCallbacks*, VkSwapchainKHR*))vkGetDeviceProcAddr(vk_fake_device, "vkCreateSwapchainKHR");

    vkDestroyDevice(vk_fake_device, vk_allocator);

    // Hook the functions
    rv = funchook_prepare(funchook, (void**)&queue_present_original, (void*)queue_present_hook);
    if (rv != 0) {
      print("Failed to prepare vkQueuePresentKHR hook\n");
      return;
    }  

    rv = funchook_prepare(funchook, (void**)&acquire_next_image_original, (void*)acquire_next_image_hook);
    if (rv != 0) {
      print("Failed to prepare vkAcquireNextImageKHR hook\n");
      return;
    }

    rv = funchook_prepare(funchook, (void**)&acquire_next_image2_original, (void*)acquire_next_image2_hook);
    if (rv != 0) {
      print("Failed to prepare vkAcquireNextImage2KHR hook\n");
      return;
    }  

    rv = funchook_prepare(funchook, (void**)&create_swapchain_original, (void*)create_swapchain_hook);
    if (rv != 0) {
      print("Failed to prepare vkCreateSwapchainKHR hook\n");
      return;
    }  

    dlclose(lib_vulkan_handle);
  }

  void* lib_sdl_handle = dlopen("libSDL3.so.0", RTLD_LAZY | RTLD_NOLOAD);

  if (!lib_sdl_handle) {
    print("Failed to load SDL3\n");
    return;
  }
 
  print("SDL3 loaded at %p\n", lib_sdl_handle);
  
  peep_events_original = (int (*)(SDL_Event*, int, SDL_EventAction, int, int))dlsym(lib_sdl_handle, "SDL_PeepEvents");
  rv = funchook_prepare(funchook, (void**)&peep_events_original, (void*)peep_events_hook);
  if (rv != 0) {
    print("Failed to prepare SDL_PeepEvents hook\n");
    return;
  }

  get_window_size_original = (void (*)(SDL_Window*, int*, int*))dlsym(lib_sdl_handle, "SDL_GetWindowSize");
  rv = funchook_prepare(funchook, (void**)&get_window_size_original, (void*)get_window_size_hook);
  if (rv != 0) {
    print("Failed to prepare SDL_GetWindowSize hook\n");
    return;
  }
  
  dlclose(lib_sdl_handle);

  rv = funchook_install(funchook, 0);
  if (rv != 0) {
    print("Non-VMT related hooks failed\n");
  }
  
}

__attribute__((destructor))
void exit() {
  if (!write_to_table(input_vtable, 6, (void*)input_create_move_original)) {
    print("Input::CreateMove failed to restore hook\n");
  }

  if (!write_to_table(input_vtable, 8, (void*)allow_camera_angle_change_original)) {
    print("Input::AllowCameraAngleChange failed to restore hook\n");
  }

  funchook_uninstall(funchook, 0);
}
