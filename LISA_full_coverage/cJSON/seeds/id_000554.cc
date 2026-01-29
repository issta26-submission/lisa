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
//<ID> 554
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = " { \"count\": 42, \"name\": \"widget\", \"flag\": false } ";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char*)cJSON_malloc(raw_len);
    memcpy(mutable_json, raw_json, raw_len);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);
    cJSON_free(mutable_json);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "active");
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double original_count = cJSON_GetNumberValue(count_item);
    double new_count = original_count + 7.5;
    cJSON_SetNumberHelper(count_item, new_count);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    size_t pretty_len = pretty ? strlen(pretty) : 0;
    (void)pretty_len;

    // step 4: Validate & Cleanup
    double extracted = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    (void)extracted;
    if (pretty) { cJSON_free(pretty); }
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}