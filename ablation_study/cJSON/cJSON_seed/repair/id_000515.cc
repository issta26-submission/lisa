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
//<ID> 515
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"fuzz\",\"enabled\":false,\"count\":2}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *ver = cJSON_CreateString("1.2.3");
    cJSON_AddItemToObject(meta, "version", ver);
    cJSON *extra_num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "extra", extra_num);
    cJSON *added_bool = cJSON_AddBoolToObject(root, "imported", 1);
    (void)added_bool;

    // step 3: Operate and Validate
    cJSON_bool has_enabled = cJSON_HasObjectItem(root, "enabled");
    double score_val = (double)has_enabled * 100.0;
    cJSON *score = cJSON_CreateNumber(score_val);
    cJSON_AddItemToObject(meta, "score", score);
    const char subarray_json[] = "[1,2,3]";
    size_t subarray_len = sizeof(subarray_json) - 1;
    cJSON *subarray = cJSON_ParseWithLengthOpts(subarray_json, subarray_len, &parse_end, 1);
    cJSON_AddItemToObject(root, "list", subarray);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}