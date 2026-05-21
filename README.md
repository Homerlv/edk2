# macOS上配置编译EDK2指南

本指南详细记录了在macOS上从零开始配置、编译EDK2并实现第一个UEFI程序的过程，包含了过程中遇到的各种问题及解决方案。

## 环境准备

### 系统要求
- macOS 10.15或更高版本
- Xcode 12或更高版本（包含Command Line Tools）
- Python 3.7或更高版本
- Git

### 安装依赖
```bash
# 安装Command Line Tools
xcode-select --install

# 安装Python（如果未安装）
brew install python

# 安装Git（如果未安装）
brew install git
```

## 1. 获取EDK2源码

### 问题1：GitHub网速过慢，update submodule失败

**解决方案：使用Gitee替换.gitmodules**

1. 克隆EDK2仓库
   ```bash
   git clone https://gitee.com/tianocore/edk2.git
   cd edk2
   ```

2. 修改.gitmodules文件，将所有GitHub链接替换为Gitee链接
   ```bash
   # 编辑.gitmodules文件
   sed -i '' 's|https://github.com/tianocore|https://gitee.com/tianocore|g' .gitmodules

   # 同步
   git submodule sync
   ```

3. 初始化并更新子模块
   ```bash
   git submodule init
   git submodule update
   ```

## 2. 配置编译环境

### 问题2：编译工具链频繁出错

#### Xcode5工具链问题
- **问题**：mtoc工具存在问题，源码存在诸多兼容问题
- **解决方案**：修改tools_def.txt配置文件，添加必要的编译器标志

#### Clang工具链问题
- **问题**：缺少lld-link工具，且难以用ld替换
- **解决方案**：切换到Xcode5工具链

### 配置步骤

1. 设置EDK2环境变量
   ```bash
   source edksetup.sh
   ```

2. 修改工具链配置
   编辑 `Conf/tools_def.txt` 文件，为XCODE5 X64编译器添加必要的标志：

   ```
   DEBUG_XCODE5_X64_CC_FLAGS   = -target x86_64-pc-win32-macho -c -g -gdwarf -Os       -Wall -Werror -Wextra -include AutoGen.h -funsigned-char -fno-ms-extensions -fno-stack-protector -fno-builtin -fshort-wchar -mno-implicit-float -mms-bitfields -Wno-unused-parameter -Wno-missing-braces -Wno-missing-field-initializers -Wno-tautological-compare -Wno-sign-compare -Wno-varargs -ftrap-function=undefined_behavior_has_been_optimized_away_by_clang -Wno-unused-but-set-variable -Wno-unused-but-set-parameter -Wno-cast-function-type-mismatch -Wno-parentheses-equality -D NO_MSABI_VA_FUNCS $(PLATFORM_FLAGS)
   NOOPT_XCODE5_X64_CC_FLAGS   = -target x86_64-pc-win32-macho -c -g -gdwarf -O0       -Wall -Werror -Wextra -include AutoGen.h -funsigned-char -fno-ms-extensions -fno-stack-protector -fno-builtin -fshort-wchar -mno-implicit-float -mms-bitfields -Wno-unused-parameter -Wno-missing-braces -Wno-missing-field-initializers -Wno-tautological-compare -Wno-sign-compare -Wno-varargs -ftrap-function=undefined_behavior_has_been_optimized_away_by_clang -Wno-unused-but-set-variable -Wno-unused-but-set-parameter -Wno-cast-function-type-mismatch -Wno-parentheses-equality -D NO_MSABI_VA_FUNCS $(PLATFORM_FLAGS)
   RELEASE_XCODE5_X64_CC_FLAGS   = -target x86_64-pc-win32-macho -c    -Os       -Wall -Werror -Wextra -include AutoGen.h -funsigned-char -fno-ms-extensions -fno-stack-protector -fno-builtin -fshort-wchar -mno-implicit-float -mms-bitfields -Wno-unused-parameter -Wno-missing-braces -Wno-missing-field-initializers -Wno-tautological-compare -Wno-sign-compare -Wno-varargs -Wno-unused-const-variable -ftrap-function=undefined_behavior_has_been_optimized_away_by_clang -Wno-cast-function-type-mismatch -Wno-parentheses-equality -D NO_MSABI_VA_FUNCS $(PLATFORM_FLAGS)
   ```

## 3. 编译测试

### 编译MdeModulePkg
```bash
build -p MdeModulePkg/MdeModulePkg.dsc -a X64 -t XCODE5 -b DEBUG
```

### 编译OVMF
```bash
build -p OvmfPkg/OvmfPkgX64.dsc -b DEBUG -t XCODE5 -a X64
```

## 4. 常见问题及解决方案

### 问题1：类型转换错误
**错误信息**：
```
error: cast from 'void *(*)(void *, const void *, UINTN)' to 'EFI_PEI_COPY_MEM' converts to incompatible function type [-Werror,-Wcast-function-type-mismatch]
```

**解决方案**：在tools_def.txt中添加 `-Wno-cast-function-type-mismatch` 标志

### 问题2：括号内相等比较警告
**错误信息**：
```
error: equality comparison with extraneous parentheses [-Werror,-Wparentheses-equality]
```

**解决方案**：在tools_def.txt中添加 `-Wno-parentheses-equality` 标志

### 问题3：汇编指令32位操作数错误
**错误信息**：
```
error: instruction requires a 32-bit register or memory operand
```

**解决方案**：修改汇编文件，将 `strict dword` 改为 `strict qword`

### 问题4：文本重定位错误
**错误信息**：
```
error: text relocations remain
```

**解决方案**：使用相对寻址，如 `lea rsp, [rel STACK_TOP]`

### 问题5：头文件缺失错误
**错误信息**：
```
error: 'libkern/OSByteOrder.h' file not found
```

**解决方案**：修改byteorder.h，使用 `__has_include` 检查头文件是否存在

## 5. 实现第一个UEFI程序

### 创建HelloWorld程序

1. 在 `MdeModulePkg` 目录下创建 `HelloWorld` 文件夹
   ```bash
   mkdir -p MdeModulePkg/HelloWorld
   ```

2. 创建以下文件：

   **HelloWorld.inf**
   ```
   [Defines]
     INF_VERSION                    = 0x00010005
     BASE_NAME                      = HelloWorld
     FILE_GUID                      = 12345678-1234-1234-1234-123456789012
     MODULE_TYPE                    = UEFI_APPLICATION
     VERSION_STRING                 = 1.0
     ENTRY_POINT                    = UefiMain
   
   [Sources]
     HelloWorld.c
   
   [Packages]
     MdePkg/MdePkg.dec
   
   [LibraryClasses]
     UefiApplicationEntryPoint
     UefiLib
   ```

   **HelloWorld.c**
   ```c
   #include <Uefi.h>
   #include <Library/UefiLib.h>
   
   EFI_STATUS
   EFIAPI
   UefiMain (
     IN EFI_HANDLE        ImageHandle,
     IN EFI_SYSTEM_TABLE  *SystemTable
     )
   {
     Print(L"Hello, UEFI World!\n");
     Print(L"Welcome to EDK2 development on macOS!\n");
     return EFI_SUCCESS;
   }
   ```

3. 编译程序
   ```bash
   build -p MdeModulePkg/MdeModulePkg.dsc -a X64 -t XCODE5 -b DEBUG -m MdeModulePkg/HelloWorld/HelloWorld.inf
   ```

### 将程序存储到镜像文件

#### macOS上生成镜像文件的问题
**问题**：macOS没有内置的mkisofs或genisoimage工具
**解决方案**：使用hdiutil创建FAT32镜像

1. 创建临时目录用于构建镜像
   ```bash
   mkdir -p efi_boot
   ```

2. 复制编译好的HelloWorld程序到临时目录
   ```bash
   cp Build/MdeModule/DEBUG_XCODE5/X64/MdeModulePkg/HelloWorld/HelloWorld/DEBUG/HelloWorld.efi efi_boot/
   ```

3. 创建FAT32镜像文件
   ```bash
   hdiutil create -fs FAT32 -volname "UEFI_BOOT" -srcfolder efi_boot uefi_boot.dmg
   ```

4. 转换为raw格式（QEMU需要）
   ```bash
   hdiutil convert uefi_boot.dmg -format UDRW -o uefi_boot.img
   ```

## 6. 配置OVMF和运行程序

### 配置OVMF

1. 确保OVMF已编译成功
   ```bash
   ls -la Build/OvmfX64/DEBUG_XCODE5/X64/OVMF.fd
   ```

2. 准备运行环境
   - OVMF.fd：UEFI固件
   - uefi_boot.img：包含HelloWorld程序的镜像

### 使用QEMU运行UEFI程序

1. 安装QEMU
   ```bash
   brew install qemu
   ```

2. 运行QEMU，加载OVMF和镜像
   ```bash
   qemu-system-x86_64 \
     -bios Build/OvmfX64/DEBUG_XCODE5/X64/OVMF.fd \
     -hda uefi_boot.img \
     -net none \
     -nographic
   ```

3. 在UEFI Shell中运行HelloWorld程序
   ```
   FS0:\> HelloWorld.efi
   ```

   你应该会看到输出：
   ```
   Hello, UEFI World!
   Welcome to EDK2 development on macOS!
   ```

### 常见问题及解决方案

#### QEMU运行问题
**问题**：QEMU启动后无法找到FS0:
**解决方案**：确保镜像文件格式正确，使用FAT32文件系统

**问题**：HelloWorld.efi找不到
**解决方案**：检查文件路径是否正确，确保程序已正确复制到镜像中

**问题**：QEMU图形界面显示问题
**解决方案**：使用 `-nographic` 参数以文本模式运行，或确保X11环境正常

## 7. 总结

在macOS上配置编译EDK2确实会遇到一些挑战，主要包括：

1. **网络问题**：GitHub网速慢，通过使用Gitee镜像解决
2. **工具链问题**：
   - Clang缺少lld-link工具
   - Xcode5工具链需要添加额外的编译器标志
   - 汇编代码需要适配64位模式
3. **兼容性问题**：
   - 类型转换警告
   - 括号内相等比较警告
   - 文本重定位错误
   - 头文件缺失问题

通过本指南提供的解决方案，你应该能够成功在macOS上配置EDK2环境并编译出UEFI程序。遇到问题时，记得参考本指南的解决方案，或者查阅EDK2官方文档。

## 参考资料
- [EDK2官方文档](https://github.com/tianocore/tianocore.github.io/wiki/EDK-II)
- [UEFI规范](https://uefi.org/specifications)
- [QEMU文档](https://www.qemu.org/docs/master/)
