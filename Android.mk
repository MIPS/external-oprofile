BUILD_OPROFILE:=false
ifeq ($(TARGET_ARCH),arm)
BUILD_OPROFILE:=true
endif
ifeq ($(TARGET_ARCH),mips)
BUILD_OPROFILE:=true
endif

ifeq ($(BUILD_OPROFILE),true)
include $(call all-subdir-makefiles)
endif
