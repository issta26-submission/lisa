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
//<ID> 123
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *copy = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *raw_item = cJSON_CreateRaw("unparsed:token");
    cJSON *retrieved = NULL;
    char *printed = NULL;
    double value = 0.0;
    cJSON_bool raw_is = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "answer", num_item);
    cJSON_AddItemToObject(root, "blob", raw_item);
    cJSON_AddNumberToObject(copy, "answer", 42.0);
    cJSON_AddItemToObject(copy, "blob", cJSON_CreateRaw("unparsed:token"));

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "answer");
    value = cJSON_GetNumberValue(retrieved);
    raw_is = cJSON_IsRaw(cJSON_GetObjectItem(root, "blob"));
    equal = cJSON_Compare(root, copy, 1);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(copy);

    // API sequence test completed successfully
    return 66;
}