function [x,y,typ] = hart_hapio_device(job,arg1,arg2)

  NDOF = 3;
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    standard_draw(arg1)
  case 'getinputs' then
    [x,y,typ]=standard_inputs(arg1)
  case 'getoutputs' then
    [x,y,typ]=standard_outputs(arg1)
  case 'getorigin' then
    [x,y]=standard_origin(arg1)
  case 'set' then
    x=arg1
    model=arg1.model;graphics=arg1.graphics;
    exprs=graphics.exprs;
    while %t do
  try
  getversion('scilab');
      [ok,numDevices,exprs]=..
      scicos_getvalue('Interface to DHD',..
        ['Number of devices'],..
      list('vec',-1),exprs)
catch
      [ok,numDevices,exprs]=..
      getvalue('Interface to DHD',..
        ['Number of devices'],..
      list('vec',-1),exprs)
end;
     if ~ok then break,end
      in=[[NDOF*numDevices]]
      out=[[NDOF*numDevices]]
      evtin=[1]
      evtout=[]
      [model,graphics,ok]=check_io(model,graphics,in,out,evtin,evtout);
      if ok then
        graphics.exprs=exprs;
        model.ipar=[numDevices;
];
        model.rpar=[];
   model.dstate=[];
        x.graphics=graphics;x.model=model
        break
      end
    end
  case 'define' then
     numDevices=1;
   model=scicos_model()
   model.sim=list('rt_hapio_device',4)
   model.in=[[NDOF*numDevices]]
   model.out=[[NDOF*numDevices]]
   model.evtin=[1]
   model.evtout=[]
   model.ipar=[numDevices;
];
   model.rpar=[];
 model.dstate=[];
 model.blocktype='d';
 model.dep_ut=[%t %t];
    exprs=[sci2exp(numDevices)]
    gr_i=['xstringb(orig(1),orig(2),[''DHD''],sz(1),sz(2),''fill'');'];
    x=standard_define([3 2],model,exprs,gr_i)
case 'readout' then
      BLOCK.version=020;
      BLOCK.name='hart_hapio_device';
      BLOCK.comp_name='rt_hapio_device';
      BLOCK.desr_short='Interface to DHD';
      BLOCK.dep_u=%t;
      BLOCK.dep_t=%t;
      BLOCK.blocktype='d';
      BLOCK.dstate='';
      BLOCK.IOmatrix=%f;
      BLOCK.inset=%t;
      BLOCK.in='[NDOF*numDevices]';
      BLOCK.outset=%t;
      BLOCK.out='[NDOF*numDevices]';
      BLOCK.evtin='1';
      BLOCK.evtout='';
      BLOCK.size='3 2';
      BLOCK.completelabel=%f;
      BLOCK.label=[39,39,68,72,68,39,39,59,110,117,109,68,101,118,105,99,101,115,10];
      BLOCK.ipar=[110,117,109,68,101,118,105,99,101,115,59,10];
      BLOCK.rpar=[];
      BLOCK.opar=[];
      BLOCK.parameter=list();
      BLOCK.parameter($+1)=[];
      BLOCK.parameter(1).name='numDevices';
      BLOCK.parameter(1).text='Number of devices';
      BLOCK.parameter(1).type='vec';
      BLOCK.parameter(1).size='-1';
      BLOCK.parameter(1).init='1';
      BLOCK.parameter(1).visible_plot=%f;
      x=BLOCK;
  end
endfunction
