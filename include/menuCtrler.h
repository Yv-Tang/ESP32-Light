#ifndef MENU_CTRLER_H
#define MENU_CTRLER_H

class MENU
{
private:
    const char *name;             // 菜单名称
    MENU *parentMenu = nullptr;   // 父菜单指针
    MENU *subMenu[15];            // 子菜单数组
    const char *lines[4];         // 菜单内容数组
    void (*func)(void) = nullptr; // 功能函数指针
    int subMenuCount = 0;         // 子菜单计数器

public:
    /// @brief 构造函数
    /// @param name 菜单名称
    /// @param parentMenu 父菜单指针（可选）
    /// @param func 功能函数指针（可选）
    MENU(const char *name, MENU *parentMenu = nullptr, void (*func)(void) = nullptr)
        : name(name), parentMenu(parentMenu), func(func) {}

    /// @brief 获取当前菜单名称-返回私有变量name
    const char *getMenuName() const
    {
        return this->name;
    }

    /// @brief 修改当前菜单名称-修改私有变量name
    void setMenuName(const char *name)
    {
        this->name = name;
    }

    /// @brief 添加单个MENU对象为子菜单-将addMenu对象添加到私有变量subMenu数组中
    void addSubMenu(MENU *addMenu)
    {
        if (addMenu == nullptr)
            return;
        addMenu->parentMenu = this;
        subMenu[subMenuCount++] = addMenu;
    }

    /// @brief 获取子菜单指针-返回私有变量subMenu数组中第index个元素
    MENU *getSubMenu(int index) const
    {
        return (index >= 0 && index < subMenuCount) ? subMenu[index] : nullptr;
    }

    /// @brief 获取父菜单指针-返回私有变量parentMenu
    MENU *getParentMenu() const
    {
        return this->parentMenu;
    }

    /// @brief 设置MENU对象的功能函数-将func函数指针赋值给私有变量func
    void setEvent(void (*func)(void))
    {
        this->func = func;
    }

    /// @brief 渲染菜单内容-获取子菜单名称并存入私有变量lines[i]返回(上限为4)
    /// @return 菜单内容数组const char**
    const char **renderLines()
    {
        for (int i = 0; i < 4; i++)
        {
            lines[i] = subMenu[i] ? subMenu[i]->getMenuName() : "";
        }
        return lines;
    }

    /// @brief 执行功能函数-如果有功能函数则执行
    void execFunc() const
    {
        if (func)
        {
            this->func();
        }
    }
};

class OLEDStatus
{

private:
    int menuLevel = 0;                           // 菜单级数
    int cursorPos = 0;                           // 光标位置
    const char *menulines[4] = {"", "", "", ""}; // nullptr, nullptr, nullptr; // 预显示菜单内容
    MENU *currentMenu = nullptr;                 // 当前菜单对象
public:
    Adafruit_SSD1306 *Screen; // 屏幕对象
    /// @brief 构造函数-传入屏幕对象
    OLEDStatus(Adafruit_SSD1306 &oled) : Screen(&oled)
    {
        Serial.println("OLEDStatus Inited");
    }

    /// @brief  启动菜单-设置当前菜单对象为rootMenu并更新菜单内容
    void start(MENU &rootMenu)
    {
        currentMenu = &rootMenu;
        updateMenu(currentMenu);
        updateScreen();
    }

    /// @brief 更新菜单内容-更新currentMenu(当前菜单对象)并获取子菜单名称并存入menulines[i]
    void updateMenu(MENU *menu, bool forced = true, bool Screen = false)
    {
        if (menu == nullptr)
            return;
        currentMenu = menu;
        if (forced)
            cursorPos = 0;
        const char **lines = currentMenu->renderLines();
        for (int i = 0; i < 4; i++)
        {
            menulines[i] = lines[i];
        }
        if (Screen)
            updateScreen();
    }

    /// @brief 更新屏幕显示(包含光标)-按行打印私有变量menulines[i]
    void updateScreen() const
    {
        if (!Screen)
            return;
        Screen->clearDisplay();
        Screen->setCursor(1, 0);
        Screen->setTextColor(WHITE);
        Screen->setTextSize(2);
        for (int i = 0; i < 4; i++)
        {
            if (i == cursorPos)
            {
                Screen->printf(">%s", menulines[i]);
            }
            else
            {
                Screen->printf(" %s", menulines[i]);
            }
            Screen->println();
        }
        Screen->display();
    }

    /// @brief 向下移动光标-如果光标位置在最后一行则移动到最上方
    void moveDown()
    {
        if (menulines[cursorPos + 1] == nullptr || menulines[cursorPos + 1] == "" || cursorPos > 2)
        {
            cursorPos = 0;
        }
        else
        {
            cursorPos++;
        }
        updateScreen();
    };

    /// @brief 向上移动光标-如果光标位置在最上方则移动到第一行
    void moveUp()
    {
        if (cursorPos != 0)
            cursorPos--;
        updateScreen();
    };

    /// @brief 选择当前菜单项-如果有功能函数则执行
    void select()
    {
        MENU *subMenu = currentMenu->getSubMenu(cursorPos);
        if (subMenu)
            subMenu->execFunc();
    }

    /// @brief 返回上一级菜单-如果有父菜单则返回父菜单，否则返回NULL
    MENU *back()
    {
        MENU *parent = currentMenu->getParentMenu();

        if (parent)
            currentMenu = parent;
        updateMenu(currentMenu);
        updateScreen();
        return parent;
    };
};

#endif