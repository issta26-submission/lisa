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
//<ID> 1209
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", flag);
    cJSON *answer = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "answer", answer);
    cJSON *maybe = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe);

    // step 2: Configure
    cJSON *note = cJSON_CreateString("configured");
    cJSON_AddItemToObject(root, "note", note);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON *maybe_ptr = cJSON_GetObjectItem(root, "maybe");
    cJSON_bool was_null = cJSON_IsNull(maybe_ptr);
    double base = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "answer"));
    double final_val = base + (double)was_null + (double)((unsigned char)((char *)scratch)[0]);
    cJSON *final_num = cJSON_CreateNumber(final_val);
    cJSON_AddItemToObject(root, "final", final_num);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}