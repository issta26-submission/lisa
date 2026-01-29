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
//<ID> 1709
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *user = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "user", user);
    cJSON_AddStringToObject(user, "name", "tester");
    cJSON_AddNumberToObject(user, "score", 99.5);
    cJSON_AddFalseToObject(user, "active");
    float values_arr[3] = {1.5f, 2.5f, 3.5f};
    cJSON *farr = cJSON_CreateFloatArray(values_arr, 3);
    cJSON_AddItemToObject(root, "values", farr);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "version", 2.0);
    cJSON_AddStringToObject(meta, "status", "configured");

    // step 3: Operate and Validate
    cJSON *retr_name = cJSON_GetObjectItem(user, "name");
    const char *name_val = cJSON_GetStringValue(retr_name);
    cJSON *retr_score = cJSON_GetObjectItem(user, "score");
    double score = cJSON_GetNumberValue(retr_score);
    int arr_size = cJSON_GetArraySize(farr);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_val ? name_val[0] : 'Y';
    buffer[2] = (char)('0' + (arr_size % 10));
    buffer[3] = (char)('0' + ((int)score % 10));
    (void)buffer;
    (void)score;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}