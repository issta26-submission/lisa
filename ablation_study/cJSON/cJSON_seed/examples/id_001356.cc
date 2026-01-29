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
//<ID> 1356
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.71);
    cJSON *n3 = cJSON_CreateNumber(42.0);
    cJSON *msg = cJSON_CreateString("fuzz test");

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);
    cJSON_AddItemToObject(root, "message", msg);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(values, 0);
    double v1 = cJSON_GetNumberValue(first);
    cJSON *second = cJSON_GetArrayItem(values, 1);
    double v2 = cJSON_GetNumberValue(second);
    double sum = v1 + v2;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum_first_two", sum_item);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *printed_copy = cJSON_CreateString(printed);
    cJSON_AddItemToObject(root, "printed_unformatted", printed_copy);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}