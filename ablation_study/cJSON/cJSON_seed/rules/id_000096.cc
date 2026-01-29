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
//<ID> 96
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *root_copy = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *num_copy = cJSON_CreateNumber(42.0);
    cJSON *raw = cJSON_CreateRaw("{\"raw\":true}");
    cJSON *raw_copy = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_bool equal = 0;
    char *printed = NULL;
    double extracted_number = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "number", num);
    cJSON_AddItemToObject(root, "rawdata", raw);
    cJSON_AddItemToObject(root_copy, "number", num_copy);
    cJSON_AddItemToObject(root_copy, "rawdata", raw_copy);

    // step 3: Operate / Validate
    equal = cJSON_Compare(root, root_copy, 1);
    printed = cJSON_PrintUnformatted(root);
    extracted_number = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "number"));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(root_copy);

    // API sequence test completed successfully
    return 66;
}