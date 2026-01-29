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
//<ID> 1572
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *config = NULL;
    cJSON *value = NULL;
    cJSON *dup_root = NULL;
    cJSON *extracted_num = NULL;
    char *printed = NULL;
    double numval = 0.0;
    cJSON_bool cmp = 0;

    // step 2: Setup / Configure
    parsed = cJSON_Parse("{\"config\":{\"value\":42},\"copy\":{\"value\":42}}");
    config = cJSON_GetObjectItem(parsed, "config");
    value = cJSON_GetObjectItem(config, "value");
    numval = cJSON_GetNumberValue(value);
    dup_root = cJSON_Duplicate(parsed, 1);

    // step 3: Operate / Validate
    extracted_num = cJSON_CreateNumber(numval);
    cJSON_AddItemToObject(dup_root, "extracted", extracted_num);
    cmp = cJSON_Compare(parsed, dup_root, 1);
    printed = cJSON_PrintUnformatted(dup_root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(dup_root);
    // API sequence test completed successfully
    return 66;
}