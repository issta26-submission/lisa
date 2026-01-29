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
//<ID> 1093
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *json_text = "{\"name\":\"initial\"}";
    cJSON *parsed = cJSON_ParseWithLength(json_text, (size_t)18);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    char *previous = cJSON_SetValuestring(name_item, "updated");
    cJSON_AddItemToObject(root, "source", parsed);

    // step 3: Operate and Validate
    cJSON *num_item = cJSON_AddNumberToObject(root, "count", 123.0);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON *got_count = cJSON_GetObjectItemCaseSensitive(root, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = unformatted[0];
    ((char *)scratch)[1] = (char)('0' + ((int)count_val % 10));
    cJSON_free(unformatted);

    // step 4: Cleanup
    cJSON_free(scratch);
    (void)previous;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}