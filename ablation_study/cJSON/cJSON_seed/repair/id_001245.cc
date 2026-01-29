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
//<ID> 1245
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_Parse("{\"init\":\"one\"}");
    cJSON *s = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(root, "field", s);

    // step 2: Configure
    char *updated = cJSON_SetValuestring(s, "beta");

    // step 3: Operate and Validate
    cJSON *got = cJSON_GetObjectItem(root, "field");
    cJSON_bool is_string = cJSON_IsString(got);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    (void)updated;
    (void)is_string;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}