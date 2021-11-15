/******************************************************************************
**  dwg2dxf - Program to convert dwg/dxf to dxf(ascii & binary)              **
**                                                                           **
**  Copyright (C) 2015 José F. Soriano, rallazz@gmail.com                    **
**                                                                           **
**  This library is free software, licensed under the terms of the GNU       **
**  General Public License as published by the Free Software Foundation,     **
**  either version 2 of the License, or (at your option) any later version.  **
**  You should have received a copy of the GNU General Public License        **
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.    **
******************************************************************************/

#ifndef DX_IFACE_H
#define DX_IFACE_H

#include "drw_interface.h"
#include "libdxfrw.h"
#include "dx_data.h"

class dx_iface : public DRW_Interface {
public:
    dx_iface(){dxfW = nullptr;}
    ~dx_iface() override= default;
    bool fileImport(const std::string& fileI, dx_data *fData, bool debug);
    bool fileExport(const std::string& file, DRW::Version v, bool binary, dx_data *fData, bool debug);
    void writeEntity(DRW_Entity* e);

//reimplement virtual DRW_Interface functions

//reader part, stores all in class dx_data
    //header
    void addHeader(const DRW_Header* data) override{
        cData->headerC = *data;
    }

    //tables
    void addLType(const DRW_LType& data) override{
        cData->lineTypes.push_back(data);
    }
    void addLayer(const DRW_Layer& data) override{
        cData->layers.push_back(data);
    }
    void addDimStyle(const DRW_Dimstyle& data) override{
        cData->dimStyles.push_back(data);
    }
    void addVport(const DRW_Vport& data) override{
        cData->VPorts.push_back(data);
    }
    void addTextStyle(const DRW_Textstyle& data) override{
        cData->textStyles.push_back(data);
    }
    void addAppId(const DRW_AppId& data) override{
        cData->appIds.push_back(data);
    }

    //blocks
    void addBlock(const DRW_Block& data) override{
        auto* bk = new dx_ifaceBlock(data);
        currentBlock = bk;
        cData->blocks.push_back(bk);
    }
    void endBlock() override{
        currentBlock = cData->mBlock;
    }

    void setBlock(const int /*handle*/) override{}//unused

    //entities
    void addPoint(const DRW_Point& data) override{
        currentBlock->ent.push_back(new DRW_Point(data));
    }
    void addLine(const DRW_Line& data) override{
        currentBlock->ent.push_back(new DRW_Line(data));
    }
    void addRay(const DRW_Ray& data) override{
        currentBlock->ent.push_back(new DRW_Ray(data));
    }
    void addXline(const DRW_Xline& data) override{
        currentBlock->ent.push_back(new DRW_Xline(data));
    }
    void addArc(const DRW_Arc& data) override{
        currentBlock->ent.push_back(new DRW_Arc(data));
    }
    void addCircle(const DRW_Circle& data) override{
        currentBlock->ent.push_back(new DRW_Circle(data));
    }
    void addEllipse(const DRW_Ellipse& data) override{
        currentBlock->ent.push_back(new DRW_Ellipse(data));
    }
    void addLWPolyline(const DRW_LWPolyline& data) override{
        currentBlock->ent.push_back(new DRW_LWPolyline(data));
    }
    void addPolyline(const DRW_Polyline& data) override{
        currentBlock->ent.push_back(new DRW_Polyline(data));
    }
    void addSpline(const DRW_Spline* data) override{
        currentBlock->ent.push_back(new DRW_Spline(*data));
    }
    // ¿para que se usa?
    void addKnot(const DRW_Entity& data) override{(void)data;}

    void addInsert(const DRW_Insert& data) override{
        currentBlock->ent.push_back(new DRW_Insert(data));
    }
    void addTrace(const DRW_Trace& data) override{
        currentBlock->ent.push_back(new DRW_Trace(data));
    }
    void add3dFace(const DRW_3Dface& data) override{
        currentBlock->ent.push_back(new DRW_3Dface(data));
    }
    void addSolid(const DRW_Solid& data) override{
        currentBlock->ent.push_back(new DRW_Solid(data));
    }
    void addMText(const DRW_MText& data) override{
        currentBlock->ent.push_back(new DRW_MText(data));
    }
    void addText(const DRW_Text& data) override{
        currentBlock->ent.push_back(new DRW_Text(data));
    }
    void addDimAlign(const DRW_DimAligned *data) override{
        currentBlock->ent.push_back(new DRW_DimAligned(*data));
    }
    void addDimLinear(const DRW_DimLinear *data) override{
        currentBlock->ent.push_back(new DRW_DimLinear(*data));
    }
    void addDimRadial(const DRW_DimRadial *data) override{
        currentBlock->ent.push_back(new DRW_DimRadial(*data));
    }
    void addDimDiametric(const DRW_DimDiametric *data) override{
        currentBlock->ent.push_back(new DRW_DimDiametric(*data));
    }
    void addDimAngular(const DRW_DimAngular *data) override{
        currentBlock->ent.push_back(new DRW_DimAngular(*data));
    }
    void addDimAngular3P(const DRW_DimAngular3p *data) override{
        currentBlock->ent.push_back(new DRW_DimAngular3p(*data));
    }
    void addDimOrdinate(const DRW_DimOrdinate *data) override{
        currentBlock->ent.push_back(new DRW_DimOrdinate(*data));
    }
    void addLeader(const DRW_Leader *data) override{
        currentBlock->ent.push_back(new DRW_Leader(*data));
    }
    void addHatch(const DRW_Hatch *data) override{
        currentBlock->ent.push_back(new DRW_Hatch(*data));
    }
    void addViewport(const DRW_Viewport& data) override{
        currentBlock->ent.push_back(new DRW_Viewport(data));
    }
    void addImage(const DRW_Image *data) override{
        auto *img = new dx_ifaceImg(*data);
        currentBlock->ent.push_back(new dx_ifaceImg(*data));
        cData->images.push_back(img);
    }

    void linkImage(const DRW_ImageDef *data) override{
        duint32 handle = data->handle;
        std::string path(data->name);
        for (auto & image : cData->images){
            if (image->ref == handle){
                dx_ifaceImg *img = image;
                img->path = path;
            }
        }
    }

//writer part, send all in class dx_data to writer
    void addComment(const char* /*comment*/) override{}
    void addPlotSettings(const DRW_PlotSettings *data) override {
        (void)data;
        // default implementation for new DRW_Interface method
    }

    virtual void writeHeader(DRW_Header& data){
        //complete copy of header vars:
        data = cData->headerC;
        //or copy one by one:
//        for (auto it=cData->headerC.vars.begin(); it != cData->headerC.vars.end(); ++it)
//            data.vars[it->first] = new DRW_Variant( *(it->second) );
    }

    void writeBlocks() override{
        //write each block
        for (const auto block : cData->blocks){
            dxfW->writeBlock(block);
            //and write each entity in block
            for (auto entity : block->ent) {
                writeEntity(entity);
            }
        }
    }
    //only send the name, needed by the reader to prepare handles of blocks & blockRecords
    void writeBlockRecords() override{
        for (const auto * block : cData->blocks)
            dxfW->writeBlockRecord(block->name);
    }
    //write entities of model space and first paper_space
    void writeEntities() override{
        for (const auto * it : cData->mBlock->ent)
            writeEntity(it);
    }
    void writeLTypes() override{
        for (auto & lineType : cData->lineTypes)
            dxfW->writeLineType(&lineType);
    }
    void writeLayers() override{
        for (const auto & layer : cData->layers)
            dxfW->writeLayer(&layer);
    }
    void writeTextstyles() override{
        for (const auto & textStyle : cData->textStyles)
            dxfW->writeTextstyle(&textStyle);
    }
    void writeVports() override{
        for (std::list<DRW_Vport>::iterator it=cData->VPorts.begin(); it != cData->VPorts.end(); ++it)
            dxfW->writeVport(&(*it));
    }
    void writeDimstyles() override{
        for (std::list<DRW_Dimstyle>::iterator it=cData->dimStyles.begin(); it != cData->dimStyles.end(); ++it)
            dxfW->writeDimstyle(&(*it));
    }
    void writeObjects() override {
        // default implementation for new DRW_Interface method
    }
    void writeAppId() override{
        for (std::list<DRW_AppId>::iterator it=cData->appIds.begin(); it != cData->appIds.end(); ++it)
            dxfW->writeAppId(&(*it));
    }

    dxfRW* dxfW; //pointer to writer, needed to send data
    dx_data* cData{}; // class to store or read data
    dx_ifaceBlock* currentBlock{};
};

#endif // DX_IFACE_H
