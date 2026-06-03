#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Hello World!\n");
    
    // 新增：获取用户键盘输入并输出
    char input[100];
    printf("请输入任意内容：");
    scanf("%s", input);
    printf("你输入的内容是：%s\n", input);
    
    system("pause");
    return 0;
}
