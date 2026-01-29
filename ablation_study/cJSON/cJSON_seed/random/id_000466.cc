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
//<ID> 466
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *year_str_item = (cJSON *)0;
    char *printed_num = (char *)0;
    char *printed_root = (char *)0;
    double num_value = 0.0;
    cJSON_bool has_year_str = 0;
    char json_text[] = " { \"a\" : 1 , \"b\" : 2 } ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(2025.0);
    str_item = cJSON_CreateString("initial");

    // step 3: Configure
    cJSON_AddItemToObject(root, "year", num_item);
    cJSON_AddItemToObject(root, "label", str_item);

    // step 4: Operate
    num_value = cJSON_GetNumberValue((const cJSON *const)num_item);
    printed_num = cJSON_Print((const cJSON *)num_item);
    year_str_item = cJSON_CreateString((const char *)printed_num);
    cJSON_AddItemToObject(root, "year_str", year_str_item);
    cJSON_Minify(json_text);
    has_year_str = cJSON_HasObjectItem((const cJSON *)root, "year_str");

    // step 5: Validate
    printed_root = cJSON_PrintUnformatted((const cJSON *)root);
    (void)num_value;
    (void)has_year_str;
    (void)json_text;
    (void)printed_root;

    // step 6: Cleanup
    cJSON_free((void *)printed_num);
    cJSON_free((void *)printed_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}