#include "item.h"

Item::Item(int stack, int amount) : stack{stack}, currentAmount{amount}
{
}

int Item::GetStack() const
{
	return stack;
}

int Item::GetCurrentAmount() const
{
	return currentAmount;
}

void Item::SetCurrentAmount(int amount)
{
	currentAmount = amount;
}

bool Item::IsEmpty() const
{
	return stack == 0 && currentAmount == 0;
}