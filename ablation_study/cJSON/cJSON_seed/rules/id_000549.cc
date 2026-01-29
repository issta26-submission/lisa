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
//<ID> 549
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"person\":{\"name\":\"Bob\",\"age\":30}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *created_num = cJSON_CreateNumber(1234.5);
    char *printed = NULL;
    cJSON *retrieved = NULL;
    double retrieved_value = 0.0;
    double combined_value = 0.0;
    char first_char = '\0';

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "created_num", created_num);
    cJSON_AddNumberToObject(root, "added_num", 78.9);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    retrieved = cJSON_GetObjectItem(root, "added_num");
    retrieved_value = cJSON_GetNumberValue(retrieved);
    first_char = printed[0];
    combined_value = retrieved_value + (double)first_char;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}