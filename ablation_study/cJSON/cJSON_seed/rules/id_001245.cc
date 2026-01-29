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
//<ID> 1245
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"value\":3.1415,\"flag\":true,\"nested\":{\"num\":42}}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = NULL;
    cJSON *val = NULL;
    cJSON *flag = NULL;
    cJSON *nested = NULL;
    cJSON *num = NULL;
    char *printed_unfmt = NULL;
    char prealloc_buf[256];
    cJSON_bool prealloc_result = 0;
    cJSON_bool is_true = 0;
    double value = 0.0;
    double nested_num = 0.0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    val = cJSON_GetObjectItem(root, "value");
    flag = cJSON_GetObjectItem(root, "flag");
    nested = cJSON_GetObjectItem(root, "nested");
    num = cJSON_GetObjectItem(nested, "num");

    // step 3: Operate / Validate
    value = cJSON_GetNumberValue(val);
    nested_num = cJSON_GetNumberValue(num);
    is_true = cJSON_IsTrue(flag);
    printed_unfmt = cJSON_PrintUnformatted(root);
    prealloc_result = cJSON_PrintPreallocated(root, prealloc_buf, (int)sizeof(prealloc_buf), 0);
    (void)json_len;
    (void)prealloc_buf;
    (void)prealloc_result;
    (void)is_true;
    (void)value;
    (void)nested_num;

    // step 4: Cleanup
    cJSON_free(printed_unfmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}