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
//<ID> 461
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
    cJSON *parsed = (cJSON *)0;
    cJSON *parsed_num = (cJSON *)0;
    char *printed = (char *)0;
    char rawjson[] = "  { \"num\" : 100, \"flag\": true }  \n";
    double val1 = 0.0;
    double val2 = 0.0;
    double total = 0.0;
    cJSON_bool has_num = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("example");
    num_item = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(root, "pi", num_item);
    cJSON_AddItemToObject(root, "name", str_item);

    // step 3: Configure
    printed = cJSON_PrintUnformatted((const cJSON *)root);

    // step 4: Operate
    val1 = cJSON_GetNumberValue((const cJSON *const)num_item);
    cJSON_Minify(rawjson);
    parsed = cJSON_Parse(rawjson);
    parsed_num = cJSON_GetObjectItem(parsed, "num");
    has_num = cJSON_HasObjectItem(parsed, "num");
    val2 = cJSON_GetNumberValue((const cJSON *const)parsed_num);
    total = val1 + val2;

    // step 5: Validate
    (void)total;
    (void)has_num;
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}