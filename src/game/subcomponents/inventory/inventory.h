#pragma once

#include "../../items/block.h"
#include "../../items/wall.h"
#include "../../items/tool.h"
#include "../../items/subtypes/pickaxe.h"
#include "../../items/subtypes/shovel.h"
#include "../../items/subtypes/axe.h"

class Inventory
{
	friend class Player;
	friend class InterfaceRenderer;

public:
	Inventory();

	int selectedItem = 0;

	void Swap(int a, int b);

	template <typename TItemType, typename TType>
	void GiveItem(TType type, int amount)
	{
		TItemType *t = new TItemType(type, 1);
		int stack = t->GetStack();
		delete t;

		if (amount > stack)
		{
			int c = static_cast<int>(amount / stack);
			int rest = amount % stack;
			for (int i = 0; i < c; i++)
			{
				GiveItem<TItemType>(type, stack);
			}
			if (rest > 0)
			{
				GiveItem<TItemType>(type, rest);
			}
		}
		else
		{
			bool itemFound = false;
			int emptyItem = -1;
			for (int i = 0; i < items.size(); i++)
			{
				if (emptyItem == -1 && items[i]->IsEmpty())
				{
					emptyItem = i;
				}

				if (TItemType *item = dynamic_cast<TItemType *>(items[i].get()))
				{
					if (static_cast<int>(type) == item->TYPE && item->GetCurrentAmount() < item->GetStack() /* move this into a function */)
					{
						item->SetCurrentAmount(item->GetCurrentAmount() + amount);
						itemFound = true;
						break;
					}
				}
			}

			if (!itemFound && emptyItem != -1)
			{
				items[emptyItem].reset(new TItemType(type, amount));
			}
		}
	}

private:
	std::vector<Ref<Item>> items;
};