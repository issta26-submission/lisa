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
//<ID> 68
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(32.5);
    cJSON_AddItemToObjectCS(child, "n1", n1);
    cJSON_AddItemToObjectCS(child, "n2", n2);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *root_flag = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObjectCS(root, "root_flag", root_flag);

    // step 2: Configure
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON_bool has_root_flag = cJSON_HasObjectItem(root, "root_flag");
    cJSON *has_child_num = cJSON_CreateNumber((double)has_child);
    cJSON *has_root_flag_num = cJSON_CreateNumber((double)has_root_flag);
    cJSON_AddItemToObject(root, "has_child", has_child_num);
    cJSON_AddItemToObjectCS(root, "has_root_flag", has_root_flag_num);

    // step 3: Operate & Validate
    cJSON *get_n1 = cJSON_GetObjectItem(child, "n1");
    cJSON *get_n2 = cJSON_GetObjectItem(child, "n2");
    double v1 = cJSON_GetNumberValue(get_n1);
    double v2 = cJSON_GetNumberValue(get_n2);
    double present_flag = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "has_child"));
    double total = v1 + v2 + present_flag;
    cJSON *sum = cJSON_CreateNumber(total);
    cJSON_AddItemToObjectCS(root, "sum", sum);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}