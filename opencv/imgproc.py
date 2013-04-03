# -*- coding: utf-8 -*-
"""
Created on Mon Apr  1 18:19:38 2013

@author: matz
"""

import cvtype
import datatype
import generator
import package

p = package.Package("imgproc", 0, 0, 1)
p.name = "OpenCV image processing"

# medianBlur

m = package.Method("medianBlur")
arg1 = package.Argument("src", "Source", cvtype.Mat(), datatype.Image())
arg2 = package.Argument("dst", "Destination", cvtype.Mat(), datatype.Image())
arg3 = package.NumericParameter("ksize", "Kernel size", cvtype.Int(), datatype.UInt32())
initIn = ("{1}->initializeImage({0}->width(), {0}->height(), "
          "{0}->stride(), {1}->data(), {0}->pixelType());"
         ).format("srcCastedData", "dstCastedData")
arg2.initIn.append(initIn)    
initOut = ("{1}->initializeImage({1}->width(), {1}->height(), "
           "{1}->stride(), {1}->data(), {0}->pixelType());"
          ).format("srcCastedData", "dstCastedData")
arg2.initOut.append(initOut)
opt = package.Option("manual")
opt.args.extend([package.Input(arg1, True), package.Output(arg2), arg3])
m.options.append(opt)
opt = package.Option("allocate")
opt.args.extend([package.Input(arg1), package.Allocation(arg2), arg3])
m.options.append(opt)
opt = package.Option("inPlace")
opt.args.extend([package.Output(arg1), package.RefInput(arg2, arg1), arg3])
m.options.append(opt)
p.methods.append(m)

### resize

m = package.Method("resize")
arg1 = package.Argument("src", "Source", cvtype.Mat(), datatype.Image())
arg2 = package.Argument("dst", "Destination", cvtype.Mat(), datatype.Image())
initIn = ("dstCastedData->initializeImage(int(m_dsizeX), "
          "int(m_dsizeY), int(m_dsizeY) * srcCastedData->pixelSize(), "
          "dstCastedData->data(), srcCastedData->pixelType());")
arg2.initIn.append(initIn)
arg3 = package.Parameter("dsize", "Size", cvtype.Int(), datatype.UInt32())
arg4 = package.Constant("fx", cvtype.Int(), 0)
arg5 = package.Constant("fy", cvtype.Int(), 0)
arg6 = package.EnumParameter("interpolation", "Interpolation")
arg6.descriptions = [package.EnumDescription("INTER_NEAREST", "Nearest neighbour"),
                     package.EnumDescription("INTER_LINEAR", "Bilinear")]
arg6.default = "INTER_LINEAR"

opt = package.Option("manual")
opt.args.extend([package.Input(arg1), package.Output(arg2),
                 arg3, arg4, arg5, arg6])
m.options.append(opt)
p.methods.append(m)

generator.generatePackageFiles(p) 