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
//<ID> 1414
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double initial_vals[] = {1.0, 2.0, 3.0};
    cJSON *double_arr = cJSON_CreateDoubleArray(initial_vals, 3);
    cJSON_AddItemToObject(root, "vals", double_arr);
    cJSON_AddNumberToObject(root, "pi", 3.141592653589793);
    const char *ver = cJSON_Version();
    cJSON *ver_ref = cJSON_CreateStringReference(ver);
    cJSON_AddItemToObject(root, "version", ver_ref);

    // step 2: Configure
    double replacement_vals[] = {10.0, 20.0};
    cJSON *replacement_arr = cJSON_CreateDoubleArray(replacement_vals, 2);
    cJSON_ReplaceItemInObject(root, "vals", replacement_arr);
    cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 3: Operate & Validate
    cJSON *got_pi = cJSON_GetObjectItem(root, "pi");
    double got_pi_val = cJSON_GetNumberValue(got_pi);
    cJSON *got_version = cJSON_GetObjectItem(root, "version");
    const char *got_ver_str = cJSON_GetStringValue(got_version);
    cJSON *got_vals = cJSON_GetObjectItem(root, "vals");
    int vals_count = cJSON_GetArraySize(got_vals);
    cJSON *first_val = cJSON_GetArrayItem(got_vals, 0);
    double first_val_num = cJSON_GetNumberValue(first_val);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    (void)got_pi_val;
    (void)got_ver_str;
    (void)vals_count;
    (void)first_val_num;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}