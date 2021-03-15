#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define Max 30

int candidateNum=0;//��ѡ����
int voteStuNum=0;//��ͶƱѧ����
char candName[][20]={};
int candId[Max];
int votes[Max][2];//[][0]]Ϊ�೤Ʊ��[][1]Ϊ֧��Ʊ
int stuId[Max];

int menu();    //�˵�

int main(){

    int vote();//ͶƱ
    void rank();//�������
    void printVote();//�����������
    int save();//����
    int checkCandId(int id);//��������ѧ���ж�Ͷ���ź�ѡ��
    int checkCandName(char x[]);//�������������ж�Ͷ���ź�ѡ��
    int checkStuId(int id);//�ж��Ƿ��ظ�ͶƱ
    int initialize();//
    int numCheck(char x[]);//�ж��ַ����Ƿ�������

    printf("��ӭ������ɲ�ѡ��ϵͳ\n");
    initialize();
    return 0;
}


int initialize(){//��ʼ��ѡ��ϵͳ
    printf("��ʼ��ϵͳ���������ѡ����Ŀ��");
    scanf("%d",&candidateNum);//��ȡ��ѡ����Ŀ
    for(int i=0;i<candidateNum;i++){
        printf("�������%d��ѡ��ѧ�ź���λ��������",i+1);
        scanf("%d %s",&candId[i],&candName[i]);
    }
    for(int i=0;i<candidateNum;i++){
        printf("��%dλ��ѡ�����ϣ�%d %s\n",i+1,candId[i],candName[i]);
    }
     return menu();
}

int numCheck(char x[]){//���x[]�Ƿ�ȫ������
    int i=0;
    while(x[i]){
        if(!isdigit(x[i])){
            return 0;//����0������ѧ�ţ�����ȫ���֣�
        }
        i++;
    }
    return 1;//����1������ѧ�ţ�ȫ���֣�

}
int checkStuId(int id){//���ѧ���Ƿ��ظ�
    for(int i=0;i<voteStuNum;i++){
        if(stuId[i]==id){
            return i;//�����ѧ����ͶƱ����Χ�ڵڼ�λͶ��Ʊ����0��ʼ����
        }
    }
    return -1;//û�ҵ��Ļ�������-1
}

int checkCandId(int id){//����ѧ�Ų��Һ�ѡ���ڵڼ�˳λ
    for(int i=0;i<candidateNum;i++){
        if(candId[i]==id){
            return i;//�ҵ��˺�ѡ��id�����غ�ѡ���������еı�ţ���0��ʼ����
        }
    }
    return -1;//û�ҵ��Ļ�������-1
}

int checkCandName(char x[]){//�����������Һ�ѡ���ڵڼ�˳λ
    for(int i=0;i<candidateNum;i++){
        if(strcmp(candName[i],x)==0){
            return i;//�ҵ��˺�ѡ��id�����غ�ѡ���������еı�ţ���0��ʼ����
        }
    }
    return -1;//û�ҵ��Ļ�������-1
}


void printVote(){//�����ǰͶƱ���
    printf("��ǰ�����ͶƱ��ѧ��������ѧ�ţ�\n");
    printf("��ǰ�����ͶƱ��ѧ��������%d\n",voteStuNum);
    printf("ѧ�ţ�\n");
    for(int i=0;i<voteStuNum;i++){
        printf("%d\n",stuId[i]);
    }
    printf("��ǰ��ѡ�˵�Ʊ�����\n");
    for(int i=0;i<candidateNum;i++){
        printf("%d  %s�İ೤Ʊ��Ϊ%d����֧��Ʊ��Ϊ��%d\n",candId[i],candName[i],votes[i][0],votes[i][1]);
    }
//    return menu();
}

void rank(){//�������
    int rankNum[2][candidateNum];
    int monVote[candidateNum];//�೤Ʊ��
    int parVote[candidateNum];//֧��Ʊ��
    int a;
    for(int i=0;i<candidateNum;i++){
        rankNum[0][i]=i;//�೤����
        monVote[i]=votes[i][0];

        rankNum[1][i]=i;//֧������
        parVote[i]=votes[i][1];
    }
    for(int i=0;i<candidateNum;i++){
        for(int j=i+1;j<candidateNum;j++){
            if(monVote[i]<monVote[j]){//�ж�ÿ���˰೤Ʊ����ð������
                a=rankNum[0][i];
                rankNum[0][i]=rankNum[0][j];
                rankNum[0][j]=a;
                a=monVote[i];
                monVote[i]=monVote[j];
                monVote[j]=a;
            }
            if(parVote[i]<parVote[j]){//�ж�ÿ������֧��Ʊ����ð������
                a=rankNum[1][i];
                rankNum[1][i]=rankNum[1][j];
                rankNum[1][j]=a;
                a=parVote[i];
                parVote[i]=parVote[j];
                parVote[j]=a;
            }
        }
    }
    printf("�೤�����\n");
    for(int i=0;i<candidateNum;i++){
        printf("%d  %s�İ೤Ʊ��Ϊ%d/%d\n",candId[rankNum[0][i]],candName[rankNum[0][i]],votes[rankNum[0][i]][0],voteStuNum);
    }
    printf("��֧������\n");
    for(int i=0;i<candidateNum;i++){
        printf("%d  %s����֧��Ʊ��Ϊ%d/%d\n",candId[rankNum[1][i]],candName[rankNum[1][i]],votes[rankNum[1][i]][1],voteStuNum);
    }
    menu();
}

//vote ������һ�����û��
int vote(){
    int id=1;

    while (id!=-1){
        printf("������ѧ�Ž���ͶƱ������-1��������");
        scanf("%d",&id);
        if(id==-1){//�ж��Ƿ����ͶƱ
            printVote();//��ӡͶƱ������ͶƱѧ��
            rank();//��ӡ����
            return menu();
//            exit(0);
        } else if(checkStuId(id)!=-1){//����ͶƱѧ���������ҵ�����ѧ��
//            votes[checkId(stuId)]++;//��Ӧ��ѡ�˵�Ʊ��+1
            printf("��ѧ����ͶƱ\n");
            id=1;
        }else{
            stuId[voteStuNum]=id;
            printVote();
            int a=0,b=0,Monid=0;//Monid��֧��id
            while (a!=-1){
                printf("��������ѡ��İ೤��");
                char x[20]={};
                scanf("%s",x);
                //printf("���õ�%s\n",x);
                if(numCheck(x)==1){//����ȫΪ���֣�ѧ��
                    id=atoi(x);//�ַ���תint
                    a=checkCandId(id);
                }else{
                    a=checkCandName(x);//���벻ȫΪ���֣�����
                }
                //printf("��ʱ%d\n",a);
                if(a>-1){
                    votes[a][0]++;//��aλ��ѡ�˰೤Ʊ+1
                    while (b!=-1){
                        printf("��������ѡ�����֧�飺");
                        char x2[20]={};
                        scanf("%s",x2);
                        if(numCheck(x2)==1){//����ȫΪ���֣�ѧ��
                            Monid=atoi(x2);//�ַ���תint
                            b=checkCandId(Monid);
                        }else{
                            b=checkCandName(x2);//���벻ȫΪ���֣�����
                        }
//                        scanf("%d",&Monid);
//                        b=checkCandId(Monid);
                        if(a==b){
                            printf("ѡ�����֧����೤��������ͬһ�ˣ����������롣\n");
                        }else if(b==-1){
                            printf("����������Ϣ�Ƿ��ں�ѡ�˵��С�\n");
                        } else{
                            votes[b][1]++;
                            break;//����ѭ��
                        }
//                        b=checkCandId(id);
                    }
                    printf("�������ͶƱ��лл��\n");
                    voteStuNum++;
                    break;//����ѭ��
                }else{
                    printf("����������Ϣ�Ƿ��ں�ѡ�˵��С�\n");
                }
            }
        }
    }
}

int save(){
    FILE  *fp = fopen("vote.txt", "w+");//���û��vote.txt���Զ�����
    if(fp==NULL)
    {
        printf("�ɼ�����ʧ�ܡ�\n");
        return 0;
    }
    fputs("ѧ�� ���� �೤ ֧��",fp);
    for(int i=0;i<candidateNum;i++){
        fprintf(fp,"%d  %s  %d  %d",candId[i],candName[i],votes[i][0],votes[i][1]);
        fputc('\r', fp);//����
        fputc('\n', fp);
    }
    fclose(fp);
    printf("�ѱ��棡\n");
    return menu();
}

int menu() {
    int num;
    printf("��ѡ���ܣ�\n");
    printf("1������ͶƱ\n");
    printf("2���鿴��ǰ����\n");
    printf("3�����浱ǰ���\n");
    printf("0���˳�\n");
    printf("��ѡ����Ҫִ�еĹ��ܣ�");
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
            printf("������δ��ͨ����δ֪��������ϵ����Ա��");
            return 1;//exit(0);

    }
    return 1;
}
