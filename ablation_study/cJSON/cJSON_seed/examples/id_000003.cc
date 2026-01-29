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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *child = NULL;
    cJSON *strItem = NULL;
    cJSON *numItem = NULL;
    cJSON_bool addCSResult = 0;
    const char *retrieved_str = NULL;
    double retrieved_num = 0.0;
    const char *sample_string = "example_value";

    // step 2: Setup
    child = cJSON_AddObjectToObject(root, "settings");
    strItem = cJSON_CreateString(sample_string);
    addCSResult = cJSON_AddItemToObjectCS(child, "label", strItem);
    numItem = cJSON_AddNumberToObject(child, "threshold", 42.0);

    // step 3: Operate → Validate
    cJSON_DeleteItemFromObject(child, "label");
    retrieved_num = cJSON_GetNumberValue(cJSON_GetObjectItem(child, "threshold"));
    retrieved_str = cJSON_GetStringValue(cJSON_GetObjectItem(child, "label"));
    int validation = (retrieved_num == 42.0) + (retrieved_str == NULL) + (addCSResult ? 1 : 0);
    (void)validation;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}