T=0.001;

K=1000;
B=30;
stoptime=30;

numDevices = 2;

z=%z;

E=syslin(T,K+B*(z-1)/T/z);
E=-diag(E*ones(3*numDevices,1));
ESS=tf2ss(E);
EA=ESS.a;
EB=ESS.b;
EC=ESS.c;
ED=ESS.d;
x0=zeros(size(EA,1),1);


result=importXcosDiagram('testhapio_dyn.zcos') // Carica modello scicos
Context.EA=EA; 
Context.EB=EB; 
Context.EC=EC; 
Context.ED=ED;
Context.x0=x0; 
Context.T=T;
Context.stoptime=stoptime;
scs_m.props.tf=stoptime;
Info=scicos_simulate(scs_m,list(),Context); // Lancia simulazione
