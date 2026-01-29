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
//<ID> 1570
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *newobj = NULL;
    cJSON *num_copy = NULL;
    char *out = NULL;
    double value = 0.0;
    cJSON_bool cmp = 0;

    // step 2: Setup / Configure
    const char *json = "{\"num\":42.5,\"flag\":true}";
    root = cJSON_Parse(json);
    num_item = cJSON_GetObjectItem(root, "num");
    value = cJSON_GetNumberValue(num_item);
    newobj = cJSON_CreateObject();
    cJSON_AddNumberToObject(newobj, "num_copy", value);
    num_copy = cJSON_GetObjectItem(newobj, "num_copy");

    // step 3: Operate / Validate
    cmp = cJSON_Compare(num_item, num_copy, 1);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(newobj);
    // API sequence test completed successfully
    return 66;
}