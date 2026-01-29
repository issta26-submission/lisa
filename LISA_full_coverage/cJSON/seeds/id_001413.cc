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
//<ID> 1413
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *version_str = cJSON_Version();
    cJSON *version_ref = cJSON_CreateStringReference(version_str);
    double constants_vals[] = {3.14159, 2.71828, 1.61803};
    cJSON *constants_arr = cJSON_CreateDoubleArray(constants_vals, 3);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "answer", 42.0);
    cJSON_AddItemToObject(root, "version", version_ref);
    cJSON_AddItemToObject(root, "constants", constants_arr);

    // step 3: Operate & Validate
    cJSON *got_version = cJSON_GetObjectItem(root, "version");
    const char *got_version_str = cJSON_GetStringValue(got_version);
    cJSON *got_constants = cJSON_GetObjectItem(root, "constants");
    int constants_count = cJSON_GetArraySize(got_constants);
    cJSON *first_constant = cJSON_GetArrayItem(got_constants, 0);
    double first_constant_val = cJSON_GetNumberValue(first_constant);
    (void)got_version_str;
    (void)constants_count;
    (void)first_constant_val;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}