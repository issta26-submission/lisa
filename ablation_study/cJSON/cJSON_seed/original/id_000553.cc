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
//<ID> 553
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw = "{ \"name\": \"tester\", \"value\": 0 }";
    size_t raw_len = strlen(raw);
    char *json_buf = (char *)cJSON_malloc(raw_len + 1);
    memcpy(json_buf, raw, raw_len + 1);
    cJSON_Minify(json_buf);
    cJSON *root = cJSON_Parse(json_buf);

    // step 2: Configure
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double set_result = cJSON_SetNumberHelper(value_item, 42.0);
    (void)set_result;
    cJSON_AddTrueToObject(root, "active");

    // step 3: Operate
    char *printed = cJSON_Print(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t buf_len = printed_len + 64;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    double extracted = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    (void)extracted;
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_free(json_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}