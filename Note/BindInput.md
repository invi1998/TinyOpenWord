

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

![image-20250129195515415](.\image-20250129195515415.png)

这是一个第三人称的输入映射上下文，通过WASD实现角色移动

这里我们探讨一下这样一个输入映射。对于IA_Move，他的输入值类型是Axis2D（即Verctor2D)类型，也就是（x，y)。

现在先来看W键，首先，W键按下，就会传入一个1值（默认），那么这个1是放到我们输入向量的哪个轴上呢？默认我们不做任何修改的情况下，任何单键输入都是放在X轴上，对吧，所以，此时，我们的输入向量就变成了（1,0)，对吧。但是回想一下，因为我们是WASD移动，对于一个二维坐标来说，我们的角色的朝向是指向正Y轴的，对吧。也就是说，我们按下W键，就是希望角色能向正前方移动（即Y++），可是，对于我们的目前的输入，因为我们没有对输入值做任何修正，默认所有的输入都被添加到X轴上了，所以此时我们需要通过Swizzle Input Axis Values（坐标轴交换），将轴映射顺序修改，因为目前我们的输入只有一个单值，也就是对于X,Y,Z这3个轴，谁在前谁就会被按键值初始化。而我们上面也说了，目前我们的需求就是W键修改Y轴，所以对于W来说，我们需要修改其顺序，改为YXZ，那么修改之后，传入按键回调函数的形参向量，就变成了（0,1），就达到了按下W，Y轴正增长，得到了角色前向移动的数据源。

同理，对于S，角色的后向移动，修改的是-Y。因为对于所有默认的输入，按下就是1，没按就是0，所以对于S，我们除了要交换器坐标轴，还需要将输入值改为负值（很好理解）。

那么对于A，按下A键，角色左移，在二维坐标系里，角色左移，就是向X的负轴移动（X--），因为默认情况下按键输入都是映射到第一个坐标轴上，所以对于左移按键A，我们不需要交换其坐标轴，只需要将值取负就行

那么对于D，按下D键，角色右移，在二维坐标系里，角色右移，就是向X轴的正轴移动（X++)，默认情况下输入为正，同时轴也映射到X轴，也不需要交换坐标轴，所以对于D键，我们啥也不同修改。

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

```



# 角色移动（第三人称视角）

```c++
void ATinyPlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}
```



在Unreal Engine中，角色移动的方向计算通常需要根据控制器的视角方向进行调整。以下是代码中角色移动的数学原理详解，以及为何不直接使用 `AddMovementInput(GetForward(), value)` 的原因：

---

### **1. 数学原理：基于控制器视角的移动方向计算**
#### **(1) 输入值解析**
```cpp
const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
```
- **输入值**：`InputAxisVector` 的 `Y` 分量（前后方向）和 `X` 分量（左右方向）来自玩家输入（如键盘或手柄摇杆）。

#### **(2) 获取控制器旋转**
```cpp
const FRotator Rotation = GetControlRotation();
```
- **控制器旋转**：`GetControlRotation()` 返回控制器的当前旋转（Pitch、Yaw、Roll），通常对应摄像头的方向。

#### **(3) 提取Yaw旋转**
```cpp
const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
```
- **Yaw分量**：仅保留控制器的水平旋转（Yaw），忽略上下倾斜（Pitch）和翻滚（Roll），确保移动方向始终在水平面上。

#### **(4) 计算世界空间方向向量**
```cpp
const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
```
- **旋转矩阵**：`FRotationMatrix(YawRotation)` 将Yaw旋转转换为一个3x3旋转矩阵。
- **方向向量**：
  - `GetUnitAxis(EAxis::X)`：获取旋转矩阵的 **X轴单位向量**（即控制器视角的正前方）。
  - `GetUnitAxis(EAxis::Y)`：获取旋转矩阵的 **Y轴单位向量**（即控制器视角的正右方）。

#### **(5) 应用移动输入**
```cpp
ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
```
- **移动合成**：将输入值分别与方向向量相乘，叠加到角色的移动输入中，实现以下效果：
  - `InputAxisVector.Y > 0`：角色朝控制器正前方移动。
  - `InputAxisVector.X > 0`：角色朝控制器正右方移动。

---

### **2. 为何不直接使用 `AddMovementInput(GetForward(), value)`？**
#### **(1) 方向来源的区别**
- **`GetForward()`**：
  - 返回的是 **Pawn自身坐标系的正前方**（即角色模型的朝向）。
  - 适用于需要角色模型方向与移动方向严格一致的场景（如横版2D游戏）。
- **控制器Yaw方向**：
  - 基于控制器的旋转，而非Pawn自身的旋转。
  - 适用于第一人称或第三人称跟随视角，确保移动方向始终与玩家视角一致。

#### **(2) 典型场景对比**
| **场景**                | 使用 `GetForward()` 的问题                       | 使用控制器方向的优势                   |
| ----------------------- | ------------------------------------------------ | -------------------------------------- |
| **第一人称射击**        | 角色移动方向与玩家视角不一致（如角色背对摄像头） | 移动方向始终与摄像头正前方对齐         |
| **第三人称跟随视角**    | 角色转身动画可能导致移动方向突然变化             | 移动方向平滑跟随摄像头旋转             |
| **摄像机自由旋转的RPG** | 角色模型可能面向不同方向（如战斗中侧身移动）     | 移动方向由摄像机控制，提供直观操作体验 |

#### **(3) 示例验证**
假设角色模型（Pawn）的朝向与控制器的Yaw方向存在偏移：
- **直接使用 `GetForward()`**：
  ```cpp
  // 角色移动方向基于模型自身朝向
  ControlledPawn->AddMovementInput(GetPawn()->GetActorForwardVector(), InputAxisVector.Y);
  ```
  - 若角色模型正在播放转身动画，移动方向会突然改变，导致操作不连贯。
- **使用控制器方向**：
  - 移动方向始终与玩家视角一致，无论角色模型当前朝向如何。

---

### **3. 关键总结**
- **数学本质**：通过控制器的Yaw旋转，将输入值转换为世界空间中的方向向量，实现与视角一致的移动。
- **设计意图**：分离角色模型朝向与移动方向，确保玩家操作直观性（如《刺客信条》《巫师3》的第三人称移动逻辑）。
- **灵活性**：通过代码显式控制方向，可轻松扩展复杂逻辑（如斜坡移动修正、摄像机偏移补偿）。

---

### **代码改进建议**
若需同时支持角色朝向与控制器方向对齐（如《生化危机4》的移动模式），可混合使用两种方向：
```cpp
// 根据需求混合Pawn方向与控制器方向
const FVector PawnForward = GetPawn()->GetActorForwardVector();
const FVector ControllerForward = ForwardDirection;
const FVector HybridDirection = (PawnForward + ControllerForward).GetSafeNormal();
ControlledPawn->AddMovementInput(HybridDirection, InputAxisVector.Y);
```
