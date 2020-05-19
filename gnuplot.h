//gnuplotで時系列データをリアルタイムプロットする
//gnuplotをインストールする
#ifndef ___KAL_GNUPLOT_H
#define ___KAL_GNUPLOT_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define N_PLOT 500

namespace kal{
    
class gnuplot{
private:
    FILE *gp;
    int cnt = 0;//カウント
    double t_log[N_PLOT];
    double x_log[N_PLOT];
    double n = 1;//plotする個数
    double xx_log[6][N_PLOT];

public:
    gnuplot();
    gnuplot(int n);
    ~gnuplot();
    void config();
    void rt_plot1d(double t,double x);
    void rt_plot(double t,double* x);
    void destroy_gnuplot();

};

gnuplot::gnuplot(){
    gp = popen("gnuplot", "w");
    config();  /* gnuplot configuration */
}

gnuplot::gnuplot(int n){//複数プロットするとき
    gp = popen("gnuplot", "w");
    config();  /* gnuplot configuration */
    this->n = n;
}

gnuplot::~gnuplot(){//デストラクタ
    pclose(gp);
}

void gnuplot::config()
{
    fprintf(gp,"set xrange [0.0:5.0]\n");
    fprintf(gp,"set yrange [-1.5:1.5]\n");
    fprintf(gp,"unset tics\n");
    fprintf(gp,"set xtics 10.0\n");
    fprintf(gp,"set ytics 0.5\n");
    fprintf(gp,"set xlabel 't' font 'Times, 20'\n");
    fprintf(gp,"set ylabel 'x' font 'Times, 20'\n");
    fprintf(gp,"set grid\n");
    fprintf(gp,"set border 0\n");
    //fprintf(gp,"set size square\n");
}

void gnuplot::rt_plot1d(double t,double x){
    t_log[cnt] = t;
    x_log[cnt] = x;
    cnt++;//i=0のとき1個pプロット
    // fprintf(gp,"plot");
    // for(int i=0;i<cnt;i++){
    //     if (i!=cnt-1){
    //         fprintf(gp,"'-' with points pt 2 lc %d tit '',",i);
    //     }
    //     else{
    //         fprintf(gp,"'-' with points pt 2 lc %d tit ''\n",i);
    //     }
    // }
    // for(int i=1;i<cnt;i++){
    //     fprintf(gp,"%f %f\n",t_log[i],x_log[i]);
    //     fprintf(gp,"e\n"); 
    // }
    fprintf(gp, "plot '-' with lines lc %d \n",0);
    for(int i=0; i<cnt; i++){
		fprintf(gp, "%lf, %lf\n",t_log[i],x_log[i]);
	}
	fprintf(gp, "e\n");
    fflush(gp);
    if(cnt>N_PLOT-1){
        cnt = 0;
        fprintf(gp,"set xrange [%f:%f]\n",t,t+10);
    }
}

void gnuplot::rt_plot(double t,double* x){
    t_log[cnt] = t;
    for(int i=0;i<n;i++){
        xx_log[i][cnt] = x[i];
    }
    cnt++;//i=0のとき1個pプロット
    fprintf(gp,"plot");
    for(int j=0;j<n;j++){
        if (j!=n-1){
            fprintf(gp, "'-' with lines lc %d,",j);
        }else{
            fprintf(gp, "'-' with lines lc %d\n",j);
        }
    }

    for(int j=0;j<n;j++){
        for(int i=0; i<cnt; i++){
            fprintf(gp, "%lf, %lf\n",t_log[i],xx_log[j][i]);
        }
    fprintf(gp, "e\n");
    }
    fflush(gp);

    if(cnt>N_PLOT-1){
        cnt = 0;
        fprintf(gp,"set xrange [%f:%f]\n",t,t+5);
    }
}

void gnuplot::destroy_gnuplot(){
    pclose(gp);
}



} // namespace kal

#endif