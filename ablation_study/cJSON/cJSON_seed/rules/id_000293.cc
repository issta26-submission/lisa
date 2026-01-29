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
//<ID> 293
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char input_buf[] = " { \"num\": 1.5 , \"arr\": [1, 2, 3], \"text\": \"hello\" }    ";
    cJSON *root = cJSON_Parse(input_buf);
    cJSON *new_root = cJSON_CreateObject();
    cJSON *num_item = cJSON_GetObjectItem(root, "num");
    double original_num = cJSON_GetNumberValue(num_item);
    cJSON *dup_number = cJSON_CreateNumber(original_num * 2.0);
    cJSON *raw_item = cJSON_CreateRaw("{\"raw_key\":true}");

    // step 2: Setup / Configure
    cJSON_AddItemToObject(new_root, "double", dup_number);
    cJSON_AddItemToObject(new_root, "raw", raw_item);

    // step 3: Operate / Validate
    char *printed = cJSON_PrintUnformatted(new_root);
    cJSON *parsed_again = cJSON_Parse(printed);
    cJSON *double_item = cJSON_GetObjectItem(parsed_again, "double");
    double before_adjust = cJSON_GetNumberValue(double_item);
    cJSON_SetNumberHelper(double_item, before_adjust + 0.5);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed_again);
    cJSON_Delete(new_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}