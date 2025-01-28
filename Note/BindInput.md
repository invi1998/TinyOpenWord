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

