import pandas as pd


def export(name, data, cols):
    df = pd.DataFrame.from_dict(data)
    df = df.reindex(columns=cols)
    df.to_csv(name + ".csv", header=True, index=False)


def __main__():
    cols = [
        "Processes",
        "Input Size",
        "Total Computation",
        "Total Gathering",
        "Individual Data Ingestion Min",
        "Individual Data Ingestion Max",
        "Individual Data Ingestion Avg",
        "Individual Computation Min",
        "Individual Computation Max",
        "Individual Computation Avg"
    ]

    random_data = {'Individual Data Ingestion Avg': ['0.000443', '0.000468', '0.000508', '0.000369', '0.003789', '0.010301', '0.000626', '0.000456', '0.000690', '0.000490', '0.003608', '0.007897', '0.000980', '0.000742', '0.000808', '0.000693', '0.003301', '0.008918', '0.003080', '0.001819', '0.000859', '0.000989', '0.003077', '0.010488'], 'Processes': ['8', '16', '32', '64', '128', '256', '8', '16', '32', '64', '128', '256', '8', '16', '32', '64', '128', '256', '8', '16', '32', '64', '128', '256'], 'Individual Computation Avg': ['0.000037', '0.000027', '0.000018', '0.000015', '0.000007', '0.000008', '0.000274', '0.000136', '0.000082', '0.000047', '0.000037', '0.000112', '0.003391', '0.001555', '0.000713', '0.000361', '0.000187', '0.000087', '0.036357', '0.017686', '0.008790', '0.003938', '0.002215', '0.001375'], 'Individual Computation Max': ['0.000050', '0.000046', '0.000069', '0.000035', '0.000020', '0.000064', '0.000287', '0.000148', '0.000260', '0.000073', '0.001324', '0.014006', '0.004026', '0.001855', '0.000823', '0.000427', '0.002650', '0.000177', '0.037130', '0.018154', '0.009580', '0.004859', '0.009543', '0.027797'], 'Individual Computation Min': ['0.000033', '0.000021', '0.000015', '0.000011', '0.000005', '0.000005', '0.000259', '0.000131', '0.000069', '0.000038', '0.000021', '0.000015', '0.003090', '0.001447', '0.000688', '0.000332', '0.000153', '0.000078', '0.035713', '0.017382', '0.008073', '0.003812', '0.001797', '0.000853'], 'Individual Data Ingestion Min': ['0.000351', '0.000179', '0.000107', '0.000076', '0.000072', '0.000073', '0.000237', '0.000246', '0.000100', '0.000077', '0.000069', '0.000069', '0.000573', '0.000503', '0.000108', '0.000106', '0.000079', '0.000071', '0.002420', '0.000808', '0.000316', '0.000285', '0.000128', '0.000099'], 'Input Size': ['2560', '2560', '2560', '2560', '2560', '2560', '25600', '25600', '25600', '25600', '25600', '25600', '256000', '256000', '256000', '256000', '256000', '256000', '2560000', '2560000', '2560000', '2560000', '2560000', '2560000'], 'Individual Data Ingestion Max': ['0.000579', '0.000694', '0.001101', '0.000863', '0.017356', '0.063910', '0.001207', '0.000780', '0.002536', '0.001328', '0.019647', '0.035803', '0.001413', '0.001041', '0.001993', '0.001290', '0.015770', '0.043697', '0.003670', '0.003287', '0.001466', '0.001831', '0.017877', '0.046191'], 'Total Gathering': ['0.005438', '0.017894', '0.039638', '0.015612', '0.079904', '0.329387', '0.013442', '0.023601', '0.042437', '0.039741', '0.068211', '0.108340', '0.027519', '0.029149', '0.059188', '0.028020', '0.075854', '0.098048', '0.118081', '0.138021', '0.159805', '0.167447', '0.379806', '0.856757'], 'Total Computation': ['0.005887', '0.018318', '0.040059', '0.016011', '0.093563', '0.349591', '0.014194', '0.024082', '0.042880', '0.040289', '0.071009', '0.112997', '0.031520', '0.031215', '0.060942', '0.028743', '0.079243', '0.098211', '0.156319', '0.156814', '0.168438', '0.172006', '0.382065', '0.875758']}
    sorted_data = {'Individual Data Ingestion Avg': ['0.000415', '0.000515', '0.000474', '0.000521', '0.003708', '0.010510', '0.001971', '0.000450', '0.000815', '0.000649', '0.004173', '0.012497', '0.000856', '0.000879', '0.000596', '0.000637', '0.004282', '0.009141', '0.001529', '0.001646', '0.002182', '0.021471', '0.004165', '0.010644'], 'Processes': ['8', '16', '32', '64', '128', '256', '8', '16', '32', '64', '128', '256', '8', '16', '32', '64', '128', '256', '8', '16', '32', '64', '128', '256'], 'Individual Computation Avg': ['0.000024', '0.000025', '0.000016', '0.000014', '0.000065', '0.000006', '0.000078', '0.000046', '0.000038', '0.000029', '0.000018', '0.000016', '0.000735', '0.000383', '0.000200', '0.000110', '0.000054', '0.000035', '0.008202', '0.004034', '0.002151', '0.001321', '0.000607', '0.000418'], 'Individual Computation Max': ['0.000049', '0.000095', '0.000029', '0.000029', '0.005067', '0.000036', '0.000090', '0.000048', '0.000106', '0.000042', '0.000030', '0.000098', '0.000770', '0.000444', '0.000248', '0.000145', '0.000080', '0.000069', '0.008492', '0.004591', '0.004383', '0.002418', '0.010033', '0.022621'], 'Individual Computation Min': ['0.000019', '0.000015', '0.000012', '0.000011', '0.000005', '0.000004', '0.000074', '0.000043', '0.000029', '0.000021', '0.000013', '0.000011', '0.000714', '0.000366', '0.000184', '0.000097', '0.000043', '0.000026', '0.008071', '0.003825', '0.001888', '0.000914', '0.000457', '0.000221'], 'Individual Data Ingestion Min': ['0.000092', '0.000095', '0.000099', '0.000121', '0.000077', '0.000071', '0.000311', '0.000257', '0.000400', '0.000140', '0.000070', '0.000065', '0.000645', '0.000487', '0.000103', '0.000082', '0.000079', '0.000072', '0.001028', '0.000459', '0.000293', '0.000262', '0.000135', '0.000100'], 'Input Size': ['2560', '2560', '2560', '2560', '2560', '2560', '25600', '25600', '25600', '25600', '25600', '25600', '256000', '256000', '256000', '256000', '256000', '256000', '2560000', '2560000', '2560000', '2560000', '2560000', '2560000'], 'Individual Data Ingestion Max': ['0.001001', '0.000874', '0.001029', '0.001133', '0.020021', '0.041664', '0.006180', '0.000867', '0.002306', '0.001168', '0.016979', '0.058359', '0.001045', '0.001356', '0.001411', '0.001146', '0.016696', '0.034155', '0.002803', '0.002775', '0.019163', '0.086712', '0.019691', '0.037214'], 'Total Gathering': ['0.016696', '0.011194', '0.049525', '0.015346', '0.042128', '0.169073', '0.007578', '0.018615', '0.040324', '0.009214', '0.059046', '0.132975', '0.025940', '0.028345', '0.045185', '0.034556', '0.060690', '0.160995', '0.089981', '0.088152', '0.091857', '0.161976', '0.170323', '0.460672'], 'Total Computation': ['0.017080', '0.011963', '0.049942', '0.015923', '0.053751', '0.175657', '0.008491', '0.019172', '0.040755', '0.009789', '0.061765', '0.164730', '0.027614', '0.029434', '0.046836', '0.034746', '0.065581', '0.163398', '0.099563', '0.092875', '0.109123', '0.163287', '0.183536', '0.467550']}
    reversed_data = {'Individual Data Ingestion Avg': ['0.000389', '0.000650', '0.000693', '0.000567', '0.003898', '0.008133', '0.001099', '0.000756', '0.000546', '0.000532', '0.003023', '0.010256', '0.000785', '0.000745', '0.000828', '0.000585', '0.002949', '0.008168', '0.001268', '0.001660', '0.001247', '0.000891', '0.003578', '0.010192'], 'Processes': ['8', '16', '32', '64', '128', '256', '8', '16', '32', '64', '128', '256', '8', '16', '32', '64', '128', '256', '8', '16', '32', '64', '128', '256'], 'Individual Computation Avg': ['0.000021', '0.000027', '0.000025', '0.000018', '0.000085', '0.000006', '0.000083', '0.000050', '0.000030', '0.000026', '0.000018', '0.000054', '0.000708', '0.000387', '0.000192', '0.000108', '0.000053', '0.000034', '0.008383', '0.003823', '0.001853', '0.000908', '0.000616', '0.000249'], 'Individual Computation Max': ['0.000028', '0.000096', '0.000106', '0.000058', '0.009991', '0.000059', '0.000121', '0.000087', '0.000039', '0.000041', '0.000046', '0.009691', '0.000728', '0.000571', '0.000338', '0.000133', '0.000124', '0.000055', '0.009032', '0.004455', '0.002048', '0.001001', '0.003497', '0.003616'], 'Individual Computation Min': ['0.000019', '0.000015', '0.000013', '0.000011', '0.000004', '0.000004', '0.000070', '0.000041', '0.000028', '0.000020', '0.000013', '0.000011', '0.000688', '0.000348', '0.000177', '0.000095', '0.000041', '0.000026', '0.007769', '0.003685', '0.001794', '0.000869', '0.000431', '0.000215'], 'Individual Data Ingestion Min': ['0.000101', '0.000324', '0.000198', '0.000097', '0.000076', '0.000073', '0.000730', '0.000102', '0.000082', '0.000077', '0.000071', '0.000070', '0.000189', '0.000423', '0.000484', '0.000087', '0.000076', '0.000072', '0.000977', '0.000778', '0.000399', '0.000239', '0.000127', '0.000098'], 'Input Size': ['2560', '2560', '2560', '2560', '2560', '2560', '25600', '25600', '25600', '25600', '25600', '25600', '256000', '256000', '256000', '256000', '256000', '256000', '2560000', '2560000', '2560000', '2560000', '2560000', '2560000'], 'Individual Data Ingestion Max': ['0.000668', '0.000948', '0.001113', '0.002036', '0.023213', '0.041818', '0.001316', '0.001306', '0.001524', '0.000990', '0.012890', '0.049591', '0.001067', '0.001095', '0.001289', '0.000972', '0.017718', '0.041807', '0.001913', '0.002363', '0.002196', '0.001878', '0.014188', '0.053320'], 'Total Gathering': ['0.016606', '0.024756', '0.014955', '0.033153', '0.059919', '0.096848', '0.008686', '0.026269', '0.030828', '0.016936', '0.065180', '0.230559', '0.038799', '0.024546', '0.028010', '0.025809', '0.066985', '0.152951', '0.081755', '0.084830', '0.108245', '0.097578', '0.198122', '0.553327'], 'Total Computation': ['0.017006', '0.025480', '0.016084', '0.034081', '0.067832', '0.105521', '0.009490', '0.026798', '0.032382', '0.017282', '0.071410', '0.230647', '0.039953', '0.025596', '0.028976', '0.025993', '0.067111', '0.166596', '0.091210', '0.090212', '0.110887', '0.098782', '0.198708', '0.565547']}
    export('reversed_data',reversed_data, cols)
    export('random_data',random_data,cols)
    export('sorted_data',sorted_data,cols)


__main__()
