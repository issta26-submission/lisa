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
//<ID> 1574
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *numItem = NULL;
    cJSON *newnum = NULL;
    char *out = NULL;
    const char *json = "{\"a\": 42, \"b\": {\"c\": 42}}";
    double value = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    numItem = cJSON_GetObjectItem(root, "a");
    value = cJSON_GetNumberValue(numItem);
    newnum = cJSON_CreateNumber(value);

    // step 3: Operate / Validate
    equal = cJSON_Compare(numItem, newnum, 1);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(newnum);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}