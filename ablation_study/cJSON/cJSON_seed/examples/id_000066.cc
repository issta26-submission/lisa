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
//<ID> 66
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObjectCS(root, "list", array);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(-2.25);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "meta", nested);
    cJSON_AddItemToObject(config, "flag", cJSON_CreateNumber(1.0));
    cJSON_AddItemToObjectCS(nested, "pi", cJSON_CreateNumber(3.1415));

    // step 3: Operate & Validate
    cJSON_bool has_config = cJSON_HasObjectItem(root, "config");
    cJSON_bool has_absent = cJSON_HasObjectItem(root, "absent");
    cJSON *has_config_num = cJSON_CreateNumber((double)has_config);
    cJSON *has_absent_num = cJSON_CreateNumber((double)has_absent);
    cJSON_AddItemToObjectCS(root, "has_config", has_config_num);
    cJSON_AddItemToObject(root, "has_absent", has_absent_num);
    cJSON *a0 = cJSON_GetArrayItem(array, 0);
    cJSON *a1 = cJSON_GetArrayItem(array, 1);
    double v0 = cJSON_GetNumberValue(a0);
    double v1 = cJSON_GetNumberValue(a1);
    cJSON *sum = cJSON_CreateNumber(v0 + v1);
    cJSON_AddItemToObject(root, "sum", sum);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}