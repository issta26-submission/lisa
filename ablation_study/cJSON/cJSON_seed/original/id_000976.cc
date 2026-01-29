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
//<ID> 976
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_buf[] = "  { \"name\": \"example\", \"count\": 3 }  ";
    cJSON_Minify(json_buf);
    cJSON *root = cJSON_Parse(json_buf);

    // step 2: Configure
    cJSON *true_item = cJSON_AddTrueToObject(root, "valid");
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", null_item);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 4: Validate & Cleanup
    cJSON *got_true = cJSON_GetObjectItem(root, "valid");
    cJSON *got_null = cJSON_GetObjectItem(root, "missing");
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name = cJSON_GetStringValue(name_item);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count = cJSON_GetNumberValue(count_item);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    cJSON_Delete(dup);
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}