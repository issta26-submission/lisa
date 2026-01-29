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
//<ID> 1515
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *name_item = cJSON_AddStringToObject(root, "name", "example");
    cJSON *ver_item = cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON *items_arr = cJSON_AddArrayToObject(root, "items");
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items_arr, s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items_arr, s2);
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(items_arr, n1);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(name_item, "tester");
    cJSON_SetNumberHelper(ver_item, 2.5);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    size_t buf_len = out_len + 16;
    char *scratch = (char *)cJSON_malloc(buf_len);
    memset(scratch, 0, buf_len);
    const char *name_str = cJSON_GetStringValue(name_item);
    double ver_val = cJSON_GetNumberValue(ver_item);
    int item_count = cJSON_GetArraySize(items_arr);
    scratch[0] = name_str ? name_str[0] : 'x';
    scratch[1] = (char)('0' + ((int)item_count % 10));
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = (char)('0' + (int)cJSON_IsTrue(enabled));
    scratch[4] = set_ret ? set_ret[0] : 'y';
    (void)ver_val;

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}