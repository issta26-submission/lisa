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
//<ID> 1543
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddTrueToObject(root, "active");
    cJSON *base_num = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(root, "base", base_num);
    float sample_vals[4] = {1.25f, 2.5f, 3.75f, 5.0f};
    cJSON *samples = cJSON_CreateFloatArray(sample_vals, 4);
    cJSON_AddItemToObject(root, "samples", samples);
    cJSON_AddBoolToObject(root, "flag", (cJSON_bool)0);

    // step 2: Configure
    cJSON *offset = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(root, "offset", offset);
    cJSON_AddBoolToObject(root, "enabled", (cJSON_bool)1);

    // step 3: Operate and Validate
    cJSON *base = cJSON_GetObjectItemCaseSensitive(root, "base");
    cJSON *off = cJSON_GetObjectItemCaseSensitive(root, "offset");
    double base_val = cJSON_GetNumberValue(base);
    double off_val = cJSON_GetNumberValue(off);
    double total = base_val + off_val;
    cJSON *total_num = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "total", total_num);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? (size_t)strlen(printed) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(printed_len + 16);
    memset(scratch, 0, printed_len + 16);
    scratch[0] = (char)('A' + ((int)base_val % 26));
    scratch[1] = (char)('0' + (int)off_val % 10);
    scratch[2] = printed_len ? printed[0] : '#';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}