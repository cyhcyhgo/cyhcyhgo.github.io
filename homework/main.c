#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define Max 30

int candidateNum=0;//候选人数
int voteStuNum=0;//已投票学生数
char candName[][20]={};
int candId[Max];
int votes[Max][2];//[][0]]为班长票，[][1]为支书票
int stuId[Max];

int menu();    //菜单

int main(){

    int vote();//投票
    void rank();//排名输出
    void printVote();//不按排名输出
    int save();//保存
    int checkCandId(int id);//根据输入学号判断投几号候选人
    int checkCandName(char x[]);//根据输入姓名判断投几号候选人
    int checkStuId(int id);//判断是否重复投票
    int initialize();//
    int numCheck(char x[]);//判断字符串是否有数字

    printf("欢迎来到班干部选举系统\n");
    initialize();
    return 0;
}


int initialize(){//初始化选举系统
    printf("初始化系统，请输入候选人数目：");
    scanf("%d",&candidateNum);//获取候选人数目
    for(int i=0;i<candidateNum;i++){
        printf("请输入第%d候选人学号后四位及姓名：",i+1);
        scanf("%d %s",&candId[i],&candName[i]);
    }
    for(int i=0;i<candidateNum;i++){
        printf("第%d位候选人资料：%d %s\n",i+1,candId[i],candName[i]);
    }
     return menu();
}

int numCheck(char x[]){//检查x[]是否全是数字
    int i=0;
    while(x[i]){
        if(!isdigit(x[i])){
            return 0;//返回0代表不是学号（不是全数字）
        }
        i++;
    }
    return 1;//返回1代表是学号（全数字）

}
int checkStuId(int id){//检查学号是否重复
    for(int i=0;i<voteStuNum;i++){
        if(stuId[i]==id){
            return i;//输入的学号已投票，范围在第几位投的票，从0开始计算
        }
    }
    return -1;//没找到的话，返回-1
}

int checkCandId(int id){//根据学号查找候选人在第几顺位
    for(int i=0;i<candidateNum;i++){
        if(candId[i]==id){
            return i;//找到了候选人id，返回候选人在数组中的编号，从0开始计算
        }
    }
    return -1;//没找到的话，返回-1
}

int checkCandName(char x[]){//根据姓名查找候选人在第几顺位
    for(int i=0;i<candidateNum;i++){
        if(strcmp(candName[i],x)==0){
            return i;//找到了候选人id，返回候选人在数组中的编号，从0开始计算
        }
    }
    return -1;//没找到的话，返回-1
}


void printVote(){//输出当前投票情况
    printf("当前已完成投票的学生人数和学号：\n");
    printf("当前已完成投票的学生人数：%d\n",voteStuNum);
    printf("学号：\n");
    for(int i=0;i<voteStuNum;i++){
        printf("%d\n",stuId[i]);
    }
    printf("当前候选人的票情况：\n");
    for(int i=0;i<candidateNum;i++){
        printf("%d  %s的班长票数为%d，团支书票数为：%d\n",candId[i],candName[i],votes[i][0],votes[i][1]);
    }
//    return menu();
}

void rank(){//输出排名
    int rankNum[2][candidateNum];
    int monVote[candidateNum];//班长票数
    int parVote[candidateNum];//支书票数
    int a;
    for(int i=0;i<candidateNum;i++){
        rankNum[0][i]=i;//班长排名
        monVote[i]=votes[i][0];

        rankNum[1][i]=i;//支书排名
        parVote[i]=votes[i][1];
    }
    for(int i=0;i<candidateNum;i++){
        for(int j=i+1;j<candidateNum;j++){
            if(monVote[i]<monVote[j]){//判断每个人班长票数，冒泡排序
                a=rankNum[0][i];
                rankNum[0][i]=rankNum[0][j];
                rankNum[0][j]=a;
                a=monVote[i];
                monVote[i]=monVote[j];
                monVote[j]=a;
            }
            if(parVote[i]<parVote[j]){//判断每个人团支书票数，冒泡排序
                a=rankNum[1][i];
                rankNum[1][i]=rankNum[1][j];
                rankNum[1][j]=a;
                a=parVote[i];
                parVote[i]=parVote[j];
                parVote[j]=a;
            }
        }
    }
    printf("班长结果：\n");
    for(int i=0;i<candidateNum;i++){
        printf("%d  %s的班长票数为%d/%d\n",candId[rankNum[0][i]],candName[rankNum[0][i]],votes[rankNum[0][i]][0],voteStuNum);
    }
    printf("团支书结果：\n");
    for(int i=0;i<candidateNum;i++){
        printf("%d  %s的团支书票数为%d/%d\n",candId[rankNum[1][i]],candName[rankNum[1][i]],votes[rankNum[1][i]][1],voteStuNum);
    }
    menu();
}

//vote 里面有一个输出没定
int vote(){
    int id=1;

    while (id!=-1){
        printf("请输入学号进行投票（输入-1结束）：");
        scanf("%d",&id);
        if(id==-1){//判断是否结束投票
            printVote();//打印投票人数和投票学号
            rank();//打印排名
            return menu();
//            exit(0);
        } else if(checkStuId(id)!=-1){//在已投票学号数组中找到输入学号
//            votes[checkId(stuId)]++;//对应候选人的票数+1
            printf("该学号已投票\n");
            id=1;
        }else{
            stuId[voteStuNum]=id;
            printVote();
            int a=0,b=0,Monid=0;//Monid是支书id
            while (a!=-1){
                printf("请输入你选择的班长：");
                char x[20]={};
                scanf("%s",x);
                //printf("检查得到%s\n",x);
                if(numCheck(x)==1){//输入全为数字，学号
                    id=atoi(x);//字符串转int
                    a=checkCandId(id);
                }else{
                    a=checkCandName(x);//输入不全为数字，姓名
                }
                //printf("此时%d\n",a);
                if(a>-1){
                    votes[a][0]++;//第a位候选人班长票+1
                    while (b!=-1){
                        printf("请输入你选择的团支书：");
                        char x2[20]={};
                        scanf("%s",x2);
                        if(numCheck(x2)==1){//输入全为数字，学号
                            Monid=atoi(x2);//字符串转int
                            b=checkCandId(Monid);
                        }else{
                            b=checkCandName(x2);//输入不全为数字，姓名
                        }
//                        scanf("%d",&Monid);
//                        b=checkCandId(Monid);
                        if(a==b){
                            printf("选择的团支书与班长不允许是同一人，请重新输入。\n");
                        }else if(b==-1){
                            printf("请检查输入信息是否在候选人当中。\n");
                        } else{
                            votes[b][1]++;
                            break;//跳出循环
                        }
//                        b=checkCandId(id);
                    }
                    printf("你已完成投票，谢谢！\n");
                    voteStuNum++;
                    break;//跳出循环
                }else{
                    printf("请检查输入信息是否在候选人当中。\n");
                }
            }
        }
    }
}

int save(){
    FILE  *fp = fopen("vote.txt", "w+");//如果没有vote.txt，自动创建
    if(fp==NULL)
    {
        printf("成绩保存失败。\n");
        return 0;
    }
    fputs("学号 姓名 班长 支书",fp);
    for(int i=0;i<candidateNum;i++){
        fprintf(fp,"%d  %s  %d  %d",candId[i],candName[i],votes[i][0],votes[i][1]);
        fputc('\r', fp);//换行
        fputc('\n', fp);
    }
    fclose(fp);
    printf("已保存！\n");
    return menu();
}

int menu() {
    int num;
    printf("请选择功能：\n");
    printf("1、参与投票\n");
    printf("2、查看当前排名\n");
    printf("3、保存当前结果\n");
    printf("0、退出\n");
    printf("请选择你要执行的功能：");
    scanf("%d", &num);
    switch (num) {
        case 1:
            vote();
            break;
        case 2:
            rank();
            break;
        case 3:
            save();
            break;
        case 0:
            exit(0);
        default:
            printf("功能尚未开通或发生未知错误！请联系管理员！");
            return 1;//exit(0);

    }
    return 1;
}
