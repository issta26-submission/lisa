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
//<ID> 50
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char json_buf[] = " {  \"item\": { \"value\" : 42, \"active\" : false }, \"meta\": \" test \" } ";
    size_t buf_len = sizeof(json_buf) - 1;
    cJSON *parsed = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON *item_obj = (cJSON *)0;
    cJSON *val_obj = (cJSON *)0;
    double extracted_value = 0.0;
    char *printed = (char *)0;
    cJSON_bool parsed_present = 0;

    // step 2: Normalize input by removing whitespace/comments
    cJSON_Minify(json_buf);

    // step 3: Parse the minified JSON with explicit length
    parsed = cJSON_ParseWithLength(json_buf, buf_len);

    // step 4: Configure a new root, attach parsed structure using CS API, and record presence with a bool
    root = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "payload", parsed);
    parsed_present = (parsed != (cJSON *)0);
    cJSON_AddBoolToObject(root, "parsed_ok", parsed_present);

    // step 5: Operate / Validate - extract a numeric value from the payload and add it to root
    item_obj = cJSON_GetObjectItem(parsed, "item");
    val_obj = cJSON_GetObjectItem(item_obj, "value");
    extracted_value = cJSON_GetNumberValue(val_obj);
    cJSON_AddNumberToObject(root, "extracted_value", extracted_value);

    // step 6: Finalize - produce an unformatted string and clean up all allocations
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}