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
//<ID> 555
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw = " { \"key\": 123 , \"flag\": false } ";
    size_t raw_len = strlen(raw);
    char *json_buf = (char *)cJSON_malloc(raw_len + 1);
    memcpy(json_buf, raw, raw_len + 1);
    cJSON_Minify(json_buf);
    cJSON *root = cJSON_Parse(json_buf);

    // step 2: Configure
    cJSON *num_item = cJSON_GetObjectItem(root, "key");
    cJSON_SetNumberHelper(num_item, 456.0);
    cJSON_AddTrueToObject(root, "extra_true");

    // step 3: Operate
    char *out = cJSON_Print(root);
    size_t out_len = strlen(out);
    size_t temp_len = out_len + 64;
    void *temp_buf = cJSON_malloc(temp_len);
    memset(temp_buf, 0, temp_len);

    // step 4: Validate & Cleanup
    double final_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "key"));
    (void)final_val;
    (void)out_len;
    cJSON_free(out);
    cJSON_free(json_buf);
    cJSON_free(temp_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}