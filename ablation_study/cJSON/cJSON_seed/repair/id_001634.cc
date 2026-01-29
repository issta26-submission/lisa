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
//<ID> 1634
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"list\": [\"alpha\", \"beta\"], \"flag\": false}";
    cJSON *root = cJSON_Parse(json);
    cJSON *list = cJSON_GetObjectItemCaseSensitive(root, "list");

    // step 2: Configure
    cJSON *btrue = cJSON_CreateBool(1);
    cJSON_bool added_true = cJSON_AddItemToArray(list, btrue);
    cJSON *snew = cJSON_CreateString("gamma");
    cJSON_bool added_str = cJSON_AddItemToArray(list, snew);

    // step 3: Operate and Validate
    cJSON *flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    cJSON_bool flag_is_bool = cJSON_IsBool(flag);
    cJSON_bool flag_is_true = cJSON_IsTrue(flag);
    int count = cJSON_GetArraySize(list);
    cJSON *count_num = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "count", count_num);
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (int)count);
    buffer[2] = (char)('0' + (int)flag_is_bool);
    buffer[3] = (char)('0' + (int)flag_is_true);
    (void)added_true;
    (void)added_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}