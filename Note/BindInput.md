首先对于场景里的pawn，我们可以通过如下代码设置自动接收玩家输入

```c++
AutoPossessPlayer = EAutoReceiveInput::Player0;	// 自动接收玩家输入
```



# 旧的输入绑定

虚幻引擎有轴映射的概念-AxisMappings，我们可以创建轴映射，如下：

![image-20250128143451233](.\image-20250128143451233.png)

现在一个轴映射可以有一个或者多个输入键，例如这里我们指定输入W，指定好输入后我们需要在Pawn上创建一个函数，我们可以叫该函数为MoveForward或者任何我们需要的名字，但是有一点，它必须有一个浮点类型的输入形参。

此外我们需要完成轴映射和输入回调函数之间的绑定，用以实现调用回调函数完成移动操作。那么如何完成绑定呢？在pawn类中，有这样一个函数`SetupPlayerInputComponent(UInputComponent* Input)`,在这里我们可以通过给输入轴分配回调函数，完成所有轴映射的绑定操作。

```c++
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABird::MoveForward);
}

void ABird::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}
```



# Enhanced Input-增强输入

在Unreal Engine 5的**Enhanced Input**系统中，**Modifiers（修饰器）**用于在输入事件传递到绑定的**Input Action**之前，对原始输入值进行预处理或调整。它们可以改变输入的方向、范围、精度或逻辑关系，从而更灵活地适配复杂的输入需求。以下是对所有内置Modifiers的详细解释、用法原理及实际应用场景：

---

### **1. Modifiers的作用原理**
- **执行顺序**：在Input Mapping Context中，Modifiers按从上到下的顺序依次处理输入值，最终结果传递给Input Action。
- **输入类型兼容性**：每个Modifier可能仅支持特定类型的输入（如`Axis1D`/`Axis2D`/`Axis3D`），需注意兼容性。
- **组合使用**：可通过叠加多个Modifiers实现复杂逻辑（例如先缩放再反向）。

---

### **2. 内置Modifiers详解**

#### **(1) Negate（反向）**
- **功能**：反转输入值的符号。
- **适用类型**：`Axis1D`、`Axis2D`、`Axis3D`。
- **典型场景**：
  - 反转水平/垂直轴的移动方向（例如反转鼠标Y轴）。
  - 将手柄摇杆的输入反向。
- **示例**：  
  ```cpp
  // 原始输入值：0.5 → 处理后：-0.5
  ```

---

#### **(2) Dead Zone（死区）**
- **功能**：过滤低于阈值的输入值，消除硬件噪声（如手柄摇杆漂移）。
- **参数**：
  - **Lower Bound**：下限阈值（默认0.1），低于此值的输入视为0。
  - **Upper Bound**：上限阈值（默认0.9），用于映射输入范围到[0,1]。
  - **Type**：死区类型（轴向/径向）。
- **适用类型**：`Axis1D`、`Axis2D`。
- **典型场景**：
  - 消除手柄摇杆的微小抖动。
  - 确保角色在轻微触碰摇杆时不会移动。
- **计算方式**：
  ```cpp
  // 以Axis2D为例：
  if (InputMagnitude < LowerBound) → Output = 0
  else → 将输入重新映射到[LowerBound, UpperBound] → [0, 1]
  ```

---

#### **(3) Swizzle Input Axis Values（坐标轴交换）**
- **功能**：交换输入值的坐标轴顺序（如交换X/Y轴）。
- **参数**：选择新的轴顺序（如`YZ`→`XZ`）。
- **适用类型**：`Axis2D`、`Axis3D`。
- **典型场景**：
  - 将水平移动从X轴切换到Y轴（适配特殊控制方案）。
  - 调整3D空间中的输入方向（如摄像机控制）。
- **示例**：  
  ```cpp
  // 原始输入 (X=0.2, Y=0.5) → 应用Swizzle(Y,X) → (0.5, 0.2)
  ```

---

#### **(4) Scale（缩放）**
- **功能**：按固定系数缩放输入值。
- **参数**：缩放因子（`Scale Factor`）。
- **适用类型**：`Axis1D`、`Axis2D`、`Axis3D`。
- **典型场景**：
  - 调整输入灵敏度（如鼠标移动速度加倍）。
  - 限制输入范围（如缩小摇杆的输入幅度）。
- **示例**：  
  ```cpp
  // Scale Factor = 2.0 → 输入0.3 → 输出0.6
  ```

---

#### **(5) Smooth（平滑）**
- **功能**：对输入值进行时间上的平滑处理，减少突变。
- **参数**：平滑时间（`Smoothing Time`）。
- **适用类型**：`Axis1D`、`Axis2D`、`Axis3D`。
- **典型场景**：
  - 处理键盘按键的突然启停，使移动更自然。
  - 平滑手柄摇杆的快速操作。
- **原理**：通过插值（如Lerp）逐步逼近目标值。

---

#### **(6) FOV Scaling（视场角缩放）**
- **功能**：根据摄像机视场角（FOV）调整输入值，避免不同FOV导致的操作灵敏度差异。
- **参数**：参考FOV值（`FOV Scale`）。
- **适用类型**：`Axis2D`（如鼠标输入）。
- **典型场景**：
  - 确保在不同FOV下（如瞄准时FOV缩小），鼠标移动的灵敏度保持一致。
- **原理**：根据当前FOV与参考FOV的比值缩放输入值。

---

#### **(7) To World Space（转换到世界空间）**
- **功能**：将输入方向从玩家控制的本地空间（如角色朝向）转换到世界空间。
- **适用类型**：`Axis2D`（如摇杆输入）。
- **典型场景**：
  - 角色移动时，摇杆输入始终基于世界坐标系（如按住右摇杆向右移动，无论角色朝向如何）。
- **示例**：  
  ```cpp
  // 角色面向北方时，摇杆向右（X=1）→ 世界空间东方向（X=1, Y=0）
  // 角色旋转90度后，摇杆向右（X=1）→ 世界空间南方向（X=0, Y=-1）
  ```

---

#### **(8) Invert Vector（矢量反向）**
- **功能**：反转矢量的方向。
- **适用类型**：`Axis2D`、`Axis3D`。
- **典型场景**：快速反转2D/3D输入的矢量方向（如反转重力方向）。

---

#### **(9) Clamp Axis（轴钳制）**
- **功能**：将输入值限制在指定范围内。
- **参数**：最小值和最大值。
- **适用类型**：`Axis1D`。
- **典型场景**：限制输入范围（如限制油门值为[0,1]）。

---

### **3. 高级用法与组合示例**

#### **示例1：手柄摇杆输入处理**
```plaintext
Modifiers顺序：
1. Dead Zone (Lower=0.1, Upper=0.9) → 消除漂移
2. Scale (Factor=1.5) → 提高灵敏度
3. Negate → 反转Y轴（如俯仰控制）
```

#### **示例2：鼠标输入适配不同FOV**
```plaintext
Modifiers顺序：
1. FOV Scaling (FOV Scale=90) → 保持灵敏度一致
2. Smooth (Time=0.1) → 平滑移动
```

#### **示例3：世界空间移动**
```plaintext
Modifiers顺序：
1. To World Space → 转换到世界坐标系
2. Scale (Factor=2.0) → 加速移动
```

---

### **4. 自定义Modifier**
通过继承`UInputModifier`类，可实现自定义逻辑：
```cpp
UCLASS()
class UMyCustomModifier : public UInputModifier
