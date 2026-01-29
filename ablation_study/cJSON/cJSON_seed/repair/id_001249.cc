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
//<ID> 1249
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"initial\"}";
    cJSON *root = cJSON_Parse(json);
    cJSON *s = cJSON_CreateString("created");
    cJSON_SetValuestring(s, "updated");
    cJSON_bool s_is_string = cJSON_IsString(s);

    // step 2: Configure
    cJSON_AddItemToObject(root, "added", s);
    cJSON *got = cJSON_GetObjectItem(root, "added");
    cJSON *mirror = cJSON_CreateString(cJSON_GetStringValue(got));
    cJSON_AddItemToObject(root, "mirror", mirror);

    // step 3: Operate and Validate
    cJSON_bool mirror_is_string = cJSON_IsString(mirror);
    double computed = (double)(s_is_string) + (double)(mirror_is_string);
    cJSON *computed_num = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_num);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}