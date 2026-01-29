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
//<ID> 714
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = NULL;
    cJSON *num = NULL;
    cJSON *flag = NULL;
    char *printed = NULL;
    double extracted_value = 0.0;
    cJSON_bool flag_true = 0;
    size_t printed_len = 0;

    // step 2: Setup / Configure
    child = cJSON_AddObjectToObject(root, "child");
    num = cJSON_AddNumberToObject(child, "value", 42.5);
    flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate / Validate
    extracted_value = cJSON_GetNumberValue(num);
    flag_true = cJSON_IsTrue(flag);
    printed = cJSON_PrintUnformatted(root);
    printed_len = strlen(printed);
    memcpy(scratch, printed, printed_len);
    scratch[printed_len] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}