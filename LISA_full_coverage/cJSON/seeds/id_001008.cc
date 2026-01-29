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
//<ID> 1008
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"values\":[10,20,30],\"flag\":false,\"meta\":{\"count\":3}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(initial_json, &parse_end, 1);

    // step 2: Configure
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON *second = cJSON_GetArrayItem(values, 1);
    cJSON_bool second_is_number = cJSON_IsNumber(second);
    cJSON_AddBoolToObject(root, "second_is_number", second_is_number);
    cJSON *always_true = cJSON_AddTrueToObject(root, "always_true");
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *count = cJSON_GetObjectItem(meta, "count");
    cJSON *fake = (cJSON *)cJSON_malloc(sizeof(struct cJSON));
    memset(fake, 0, sizeof(struct cJSON));
    cJSON_bool fake_is_invalid = cJSON_IsInvalid(fake);
    cJSON_AddBoolToObject(root, "fake_is_invalid", fake_is_invalid);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(root, print_buf, 512, 0);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "flag");
    cJSON_DeleteItemFromObject(meta, "count");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_free(fake);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}