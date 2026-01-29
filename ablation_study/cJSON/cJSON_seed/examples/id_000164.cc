#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 164
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, n2);
    double new_n1 = cJSON_SetNumberHelper(n1, 3.1415);
    double new_n2 = cJSON_SetNumberHelper(n2, 2.71828);
    cJSON_AddItemReferenceToArray(values, n1);

    // step 3: Operate & Validate
    cJSON *report = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "report", report);
    cJSON *sum = cJSON_CreateNumber(new_n1 + new_n2);
    cJSON_AddItemToObject(report, "sum", sum);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}