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
//<ID> 75
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

    // step 2: Configure
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(root, "values", values);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(values, 0);
    cJSON *second = cJSON_GetArrayItem(values, 1);
    double sum_val = cJSON_GetNumberValue(first) + cJSON_GetNumberValue(second);
    cJSON *sum = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    cJSON *minstring = cJSON_CreateString(printed);
    cJSON_AddItemToObject(root, "minified", minstring);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}