module Module
    submodules:
        submod: Unknown;
            display: "p=44,184;b=56,32";
        submod1: Unknown;
            display: "p=72,216;b=64,32";
        submod2: Unknown;
            display: "p=120,244;b=96,24";
        submod4: Unknown;
            display: "p=188,168;b=72,48";
        submod5: Unknown;
            display: "p=160,212;b=64,40";
        submod6: Unknown;
            display: "p=16,156;b=16,24";
    connections:
        submod4.out++ --> submod4.in++ display "m=m,0,32,0,12";
    display: "b=276,411";
endmodule

