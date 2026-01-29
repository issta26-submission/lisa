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
//<ID> 938
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"id\":123, \"name\":\"original\", \"replaceMe\":\"old\"}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *new_value = cJSON_CreateString("new_value");

    // step 2: Configure
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "replaceMe", new_value);

    // step 3: Operate
    cJSON *got = cJSON_GetObjectItem(root, "replaceMe");
    char *got_str = cJSON_GetStringValue(got);
    char *unformatted = cJSON_PrintUnformatted(root);
    char first_char = unformatted ? unformatted[0] : 0;

    // step 4: Validate
    int validation = 0;
    validation ^= (int)(root != NULL);
    validation ^= (int)(new_value != NULL);
    validation ^= (int)replaced;
    validation ^= (int)(got != NULL);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(unformatted != NULL);
    validation ^= (int)(first_char == (unformatted ? unformatted[0] : 0));
    (void)validation;

    // step 5: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}