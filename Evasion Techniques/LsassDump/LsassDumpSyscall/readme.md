## **Step-by-Step Compilation Instructions**

### **1. Install Visual Studio**
1. Download and install the **Visual Studio IDE** (Community Edition is sufficient) from [Download](https://visualstudio.microsoft.com/).
2. During installation, ensure you select the following workloads:
   - **Desktop development with C++**
   - **MSVC v142 - VS 2019 C++ x64/x86 build tools**
   - **Windows 10 SDK**
   - **Windows 11 SDK**

### **2. Add MASM (Assembly) Support**
MASM is not enabled by default in C++ projects. Here’s how to configure it:

1. Right-click the **Solution** in the Solution Explorer and select **Add > New Project**.
2. Choose **Empty Project**.
3. Add the assembly file (`Lsasssyscall.asm`) to this project:
   - Right-click **Source Files** in the Solution Explorer.
   - Select **Add > Existing Item** and choose your `Lsasssyscall.asm` file.
   - Right click at the project name > Build Dependencies > Build Customizations > Enable MASM
   - Right click at the project name > properties > Linker > System > SubSystem : Console

### **4. Build the Project**
1. Set the build configuration to **Release** or **Debug** and **x64**.
2. Press **Ctrl+Shift+B** or go to **Build > Build Solution** to compile the project.


### **5. Run the Program**
- Run the compiled executable (`LsassDumpSyscall.exe`) as an administrator.
- Ensure the required privileges are enabled for the process (e.g., SE_DEBUG_NAME).

More details: https://programminghaven.home.blog/2020/02/16/setup-an-assembly-project-on-visual-studio-2019/
