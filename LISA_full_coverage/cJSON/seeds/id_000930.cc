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
//<ID> 930
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"existing\": \"orig\"}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    float numbers[3] = {1.5f, 2.5f, 3.5f};
    cJSON *float_arr = cJSON_CreateFloatArray(numbers, 3);
    cJSON *float_arr_ref = cJSON_CreateArrayReference(float_arr);
    cJSON *note = cJSON_CreateString("generated");
    cJSON_AddItemToObjectCS(root, "floats", float_arr);
    cJSON_AddItemToObjectCS(root, "floats_ref", float_arr_ref);
    cJSON_AddItemToObjectCS(root, "note", note);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    int buf_len = 128;
    void *buf = cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    char *prebuf = (char *)buf;
    cJSON_PrintPreallocated(root, prebuf, buf_len, 0);
    cJSON_free(prebuf);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}