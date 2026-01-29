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
//<ID> 409
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    char *printed = NULL;
    char *printed_copy = NULL;
    char *tmp_buf = (char *)cJSON_malloc(64);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *name = cJSON_CreateString("alpha");
    cJSON *parsed = NULL;
    cJSON *retrieved_array = NULL;
    cJSON *newnum = NULL;

    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddBoolToObject(root, "enabled", 1);

    printed = cJSON_Print(root);
    parsed = cJSON_Parse(printed);
    retrieved_array = cJSON_GetObjectItem(parsed, "list");
    newnum = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInArray(retrieved_array, 0, newnum);
    printed_copy = cJSON_PrintUnformatted(parsed);
    tmp_buf[0] = printed[0];
    tmp_buf[1] = printed_copy[0];
    tmp_buf[2] = (char)('0' + (int)cJSON_GetArraySize(retrieved_array));
    tmp_buf[3] = '\0';

    cJSON_free(printed);
    cJSON_free(printed_copy);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    return 66;
}