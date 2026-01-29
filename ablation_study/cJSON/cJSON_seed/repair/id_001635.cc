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
//<ID> 1635
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_Parse("{\"pre\":7}");
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", arr);
    cJSON *btrue = cJSON_CreateBool(1);
    cJSON *bfalse = cJSON_CreateBool(0);
    cJSON_AddItemToArray(arr, btrue);
    cJSON_AddItemToArray(arr, bfalse);

    // step 2: Configure
    cJSON *pre_item = cJSON_GetObjectItemCaseSensitive(root, "pre");
    double pre_val = cJSON_GetNumberValue(pre_item);
    cJSON *bderived = cJSON_CreateBool(((int)pre_val) & 1);
    cJSON_AddItemToArray(arr, bderived);

    // step 3: Operate and Validate
    cJSON *flags = cJSON_GetObjectItemCaseSensitive(root, "flags");
    int count = cJSON_GetArraySize(flags);
    cJSON *second = cJSON_GetArrayItem(flags, 1);
    char *printed = cJSON_Print(second);
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (count % 10));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}