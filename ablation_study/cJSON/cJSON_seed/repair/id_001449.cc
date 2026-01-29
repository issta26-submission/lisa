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
//<ID> 1449
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON *data = cJSON_AddArrayToObject(root, "data");

    // step 2: Configure
    cJSON_AddItemToArray(data, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(data, cJSON_CreateTrue());
    cJSON_AddItemToArray(data, cJSON_CreateString("hello"));

    // step 3: Operate and Validate
    cJSON *item0 = cJSON_GetArrayItem(data, 0);
    cJSON *item1 = cJSON_GetArrayItem(data, 1);
    cJSON *item2 = cJSON_GetArrayItem(data, 2);
    double value0 = cJSON_GetNumberValue(item0);
    cJSON_bool item1_is_true = cJSON_IsTrue(item1);
    char *str2 = cJSON_GetStringValue(item2);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = out_len > 0 ? out[0] : ' ';
    scratch[1] = (char)('0' + ((int)value0 % 10));
    scratch[2] = (char)(item1_is_true ? '1' : '0');
    scratch[3] = str2 ? str2[0] : 'n';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}