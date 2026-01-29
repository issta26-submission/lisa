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
//<ID> 344
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"alpha\",\"values\":[1,2]}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON *values = cJSON_GetObjectItemCaseSensitive(root, "values");

    // step 2: Configure
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_bool add_ref_ok = cJSON_AddItemReferenceToArray(values, n3);

    // step 3: Operate
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}