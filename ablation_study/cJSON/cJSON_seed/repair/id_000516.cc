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
//<ID> 516
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"fuzz\",\"active\":false,\"meta\":{\"count\":5}}";
    const char *parse_end = NULL;
    size_t buffer_length = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, buffer_length, &parse_end, 1);

    // step 2: Configure
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *mode = cJSON_CreateString("auto");
    cJSON_AddItemToObject(settings, "mode", mode);

    // step 3: Operate
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *has_meta_flag = cJSON_CreateNumber((double)has_meta);
    cJSON_AddItemToObject(root, "has_meta_flag", has_meta_flag);
    double leftover = (double)((initial_json + buffer_length) - parse_end);
    cJSON *leftover_num = cJSON_CreateNumber(leftover);
    cJSON_AddItemToObject(root, "leftover", leftover_num);

    // step 4: Validate and Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}