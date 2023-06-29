import xlsxwriter
import os 
import math

def ignore_nan(worksheet, row, col, number, format=None):
    if math.isnan(number):
        return worksheet.write_blank(row, col, None, format)
    else:
        # Return control to the calling write() method for any other number.
        return None

def exportExcel(app):
  firstDimKeys = list(app.keys())
  counter = 0
  for i in firstDimKeys:
    while True:
      if i[counter] == '/' :
        break
      counter += 1
    break

  foldername = firstDimKeys[0][:counter] 
  experiments = []
  for file in os.listdir(foldername):
    d = os.path.join(foldername, file)
    if os.path.isdir(d):
      if 'G_' in d:
        experiments.append(d[len(foldername)+1:])
#      else:
#        experiments.append(foldername[2:].lower())

  workbook = xlsxwriter.Workbook('simBaselineExpResults/' + foldername + "_baseSimResults.xlsx")
  for i in firstDimKeys:
    dataName = ''
    for k in experiments:
      if k in i:
        dataName = k
        break 
    nofKernel = app[i]["nofKernels"]
    worksheet = workbook.add_worksheet(dataName)
    worksheet.add_write_handler(float, ignore_nan)
    worksheet.write(0, 0, "Metrics/Kernels")
    for j in range(0, nofKernel):
      metrics = list(app[i]["kernel_" + str(j)].keys())
      worksheet.write(0, j+1, "Kernel_" + str(j))
      for k in range(0, len(metrics)):
        worksheet.write(k+1, 0, metrics[k])
        worksheet.write(k+1, j+1, app[i]['kernel_' + str(j)][metrics[k]])
  workbook.close()


#  print(app)['G_PR_BASE/webNotreDame/webNotreDame.txt', 'G_PR_BASE/webStanford/webStanford.txt', 'G_PR_BASE/webBerkstan/webBerkstan.txt', 'G_PR_BASE/germanyOSM/germanyOSM.txt', 'G_PR_BASE/higgsTwitter/higgsTwitter.txt', 'G_PR_BASE/asiaOSM/asiaOSM.txt', 'G_PR_BASE/webGoogle/webGoogle.txt']
#G_PR_BASE/webNotreDame/webNotreDame.txt
#['nofKernels', 'kernel_0', 'kernel_1']
#G_PR_BASE/webStanford/webStanford.txt
#['nofKernels', 'kernel_0', 'kernel_1']
#G_PR_BASE/webBerkstan/webBerkstan.txt
#['nofKernels', 'kernel_0', 'kernel_1']
#G_PR_BASE/germanyOSM/germanyOSM.txt
#['nofKernels', 'kernel_0', 'kernel_1']
#G_PR_BASE/higgsTwitter/higgsTwitter.txt
#['nofKernels', 'kernel_0', 'kernel_1']
#G_PR_BASE/asiaOSM/asiaOSM.txt
#['nofKernels', 'kernel_0', 'kernel_1']
#G_PR_BASE/webGoogle/webGoogle.txt
#['nofKernels', 'kernel_0', 'kernel_1']
#['G_PR_WARP/coPapersDBLP/coPapersDBLP.txt', 'G_PR_WARP/higgsTwitter/higgsTwitter.txt']
#G_PR_WARP/coPapersDBLP/coPapersDBLP.txt
#['nofKernels', 'kernel_0']
#G_PR_WARP/higgsTwitter/higgsTwitter.txt
#['nofKernels', 'kernel_0']
